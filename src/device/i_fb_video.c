#include "i_video.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdint.h>
#include "v_video.h"

static FILE* fbfd = 0;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static long int screensize = 0;
static char *fbp = 0;
static uint8_t* gameScreen;


void I_InitGraphics (void)
{
    /* Open the file for reading and writing */
    fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd) {
            printf("Error: cannot open framebuffer device.\n");
            exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    /* Get fixed screen information */
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        printf("Error reading fixed information.\n");
            exit(2);
    }

    /* Get variable screen information */
        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
                printf("Error reading variable information.\n");
                exit(3);
        }

    /* Figure out the size of the screen in bytes */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    printf("Screen size is %d\n",screensize);
    printf("Vinfo.bpp = %d\n",vinfo.bits_per_pixel);

    /* Map the device to memory */
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,fbfd, 0);
    if ((int64_t)fbp == -1) {
            printf("Error: failed to map framebuffer device to memory.\n");
            exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
        
}


void I_ShutdownGraphics(void)
{
    munmap(fbp, screensize);
    close(fbfd);
}

void I_StartFrame (void)
{

}
__attribute__((packed))
struct Color
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
};

union ColorInt
{
    struct Color col;
    uint32_t raw;
};

static union ColorInt colors[256];

// Takes full 8 bit values.
void I_SetPalette (byte* palette)
{
    byte c;
    // set the X colormap entries
    for (int i=0 ; i<256 ; i++)
    {
        c = gammatable[usegamma][*palette++];
        colors[i].col.r = (c<<8) + c;
        c = gammatable[usegamma][*palette++];
        colors[i].col.g = (c<<8) + c;
        c = gammatable[usegamma][*palette++];
        colors[i].col.b = (c<<8) + c;
    }
}

void I_UpdateNoBlit (void)
{

}
int location(int x, int y)
{
    return (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
}

uint16_t colorTo16bit(struct Color col)
{
    return  (col.r >> 3) << 11 | (col.g >> 2) << 5 | (col.b >> 3);
    //return (col.b & 0x1F) << 10 | (col.g & 0x1F) << 5 | (col.r & 0x1F);
}

void I_FinishUpdate (void)
{
    for (int gy=0; gy<SCREENHEIGHT; gy++)
    {
        for (int gx=0; gx<SCREENWIDTH; gx++)
        {
            int fbPos = location(gx,gy);
            if (vinfo.bits_per_pixel == 32)
            {
                *((uint32_t*)(fbp+fbPos + 0)) = colors[*(screens[0]+gy*SCREENWIDTH+gx)].raw;
            }
            else if (vinfo.bits_per_pixel == 16)
            {
                *((uint16_t*)(fbp+fbPos)) = colorTo16bit(colors[*(screens[0]+gy*SCREENWIDTH+gx)].col);
            }
        }
    }
}

void I_ReadScreen (byte* scr)
{
    memcpy(scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}