/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <RickEnd aka Andrey Kozhinov> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer
 * ----------------------------------------------------------------------------
 */
#include "XServResDefScreen.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
#if DEBUG
    printf("Debug version\n");
#endif
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy)
    {
        fprintf(stderr, "Failed to connect XServer\n");
        exit(1);
    }
    int screen_num = DefaultScreen(dpy);
    Window root_window = XRootWindow(dpy, screen_num);
    DisplayChangeRes(dpy, screen_num, root_window);
    return 0;
}

void DisplayChangeRes(Display *dpy, int screen_num, Window root_window)
{
    printf("\t===========XServerResizeDefaultScreen===========");
    int num_sizes = 0;
    XRRScreenSize *xrrs = XRRSizes(dpy, screen_num, &num_sizes);
    XRRScreenConfiguration *conf = XRRGetScreenInfo(dpy, root_window);
    short **pos_freq = NULL;
    short original_rate = XRRConfigCurrentRate(conf);
    Rotation original_rot = 0;
    SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rot);

    pos_freq = calloc(64, sizeof(short *));
    if (pos_freq == NULL)
    {
        perror("Failed to allocate memory");
        exit(1);
    }
    for (int i = 0; i < 64; i++)
    {
        pos_freq[i] = calloc(64, sizeof(short));
        if (pos_freq[i] == NULL)
        {
            perror("Failed to allocate memory for row");
            exit(1);
        }
    }

    EchoScreenInfo(dpy, num_sizes, pos_freq, xrrs);
    printf("\tCurrent mode :");
    printf(" %2i (%4i x %4i %i Hz)\n", original_size_id, xrrs[original_size_id].width, xrrs[original_size_id].height,
           original_rate);

    int choice = 0;
    int rate_ind = ChoiceResAndRate(pos_freq, &choice, num_sizes);
    if (choice != -1)
    {
        XRRSetScreenConfigAndRate(dpy, conf, root_window, choice, RR_Rotate_0, pos_freq[choice][rate_ind], CurrentTime);
        char yes_no = 'n';
        printf("Enable res(y/n default: n): ");
        scanf("%c", &yes_no);
        if (yes_no != 'y')
            XRRSetScreenConfigAndRate(dpy, conf, root_window, original_size_id, original_rot, original_rate,
                                      CurrentTime);
    }

    // for (int i = 0; i < 64; i++)
    // {
    //     free(pos_freq[i]);
    // }
    // free(pos_freq);
    XFree(conf);
    XCloseDisplay(dpy);
}

void EchoScreenInfo(Display *dpy, int num_sizes, short **pos_freq, XRRScreenSize *xrrs)
{
    for (int i = 0; i < num_sizes; i++)
    {
        short *rates;
        int num_rates;
        printf("\n\t%2i : %4i x %4i (", i, xrrs[i].width, xrrs[i].height);
        rates = XRRRates(dpy, 0, i, &num_rates);
        for (int j = 0; j < num_rates; j++)
        {
            pos_freq[i][j] = rates[j];
            printf("%4i Hz", rates[j]);
        }
        printf(" )");
    }
    printf("\n");
}

int ChoiceResAndRate(short **pos_freq, int *choice, int num_sizes)
{
    int rate = 0;
    int count_freq = 0;

    printf("\nChoose mode (-1 for exit):\n\t");
    scanf("%d", choice);
    if (*choice < -1 || *choice >= num_sizes)
    {
        printf("Wrong mode\n");
        ChoiceResAndRate(pos_freq, choice, num_sizes);
    }
    if (*choice != -1)
    {
        while (pos_freq[*choice][count_freq] != 0)
            count_freq++;
        if (count_freq > 1)
        {
            printf("Choose rate:");
            for (int i = 0; i < count_freq; i++)
            {
                printf("\n\t%2i: %3i Hz", i, pos_freq[*choice][i]);
            }
            printf("\n\t");
            scanf("%d", &rate);
            if (rate < 0 || rate >= count_freq)
            {
                printf("Wrong rate\n");
                ChoiceResAndRate(pos_freq, choice, num_sizes);
            }
            getchar(); // Ловим \n после scanf
        }
    }
    return rate;
}