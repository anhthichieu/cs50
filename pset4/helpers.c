#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = (float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            int avgPixel = round(avg);
            image[i][j].rgbtBlue = avgPixel;
            image[i][j].rgbtGreen = avgPixel;
            image[i][j].rgbtRed = avgPixel;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int tempBlue, tempGreen, tempRed;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            tempBlue = image[i][j].rgbtBlue;
            tempGreen = image[i][j].rgbtGreen;
            tempRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;

            image[i][width - 1 - j].rgbtBlue = tempBlue;
            image[i][width - 1 - j].rgbtGreen = tempGreen;
            image[i][width - 1 - j].rgbtRed = tempRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int pixelCount = 0;
    int sumRed = 0;
    int sumGreen = 0;
    int sumBlue = 0;
    float avgBlueCalc, avgGreenCalc, avgRedCalc;
    int avgBlue, avgGreen, avgRed;

    // Extract all original RGB values of each pixel and store in an array
    int originalRGB[height][width][3];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            originalRGB[h][w][0] = image[h][w].rgbtRed;
            originalRGB[h][w][1] = image[h][w].rgbtGreen;
            originalRGB[h][w][2] = image[h][w].rgbtBlue;
        }
    }

    for (int h = 1; h < height + 1; h++)
    {
        for (int w = 0; w < width + 1; w++)
        {
            originalRGB[h][w][0] = image[h - 1][w - 1].rgbtRed;
            originalRGB[h][w][1] = image[h - 1][w - 1].rgbtGreen;
            originalRGB[h][w][2] = image[h - 1][w - 1].rgbtBlue;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            for (int i = h - 1; i <= h + 1; i++)
            {
                for (int j = w - 1; j <= w + 1; j++)
                {
                    if (!(i < 0 || i >= height || j < 0 || j >= width))
                    {
                        sumRed += originalRGB[i][j][0];
                        sumGreen += originalRGB[i][j][1];
                        sumBlue += originalRGB[i][j][2];
                        pixelCount++;
                    }
                }
            }

            avgRedCalc = (float)sumRed / pixelCount;
            avgGreenCalc = (float)sumGreen / pixelCount;
            avgBlueCalc = (float)sumBlue / pixelCount;

            avgRed = round(avgRedCalc);
            avgGreen = round(avgGreenCalc);
            avgBlue = round(avgBlueCalc);

            image[h][w].rgbtRed = avgRed;
            image[h][w].rgbtBlue = avgBlue;
            image[h][w].rgbtGreen = avgGreen;

            pixelCount = 0;
            sumBlue = 0;
            sumGreen = 0;
            sumRed = 0;
        }
    }
    return;
}

// Detect edges

// Function to calc Gx-Gy kernel
int calc_kernels(int arr[3][3][3], int channel)
{
    long GxCalc = 0;
    long GyCalc = 0;
    int kernelVal;

    //  Gx Convolutional matrix
    int Gx[3][3] =
        {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1},
        };

    //  Gy Convolutional matrix
    int Gy[3][3] =
        {
            {-1, -2, 1},
            {0, 0, 0},
            {1, 2, 1},
        };

    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            GxCalc += (arr[i][j][channel] * Gx[i][j]);
            GyCalc += (arr[i][j][channel] * Gy[i][j]);
        }
    }

    float kernelValCalc = (float)sqrt((long)pow(GxCalc, 2) + (long)pow(GyCalc, 2));
    kernelVal = round(kernelValCalc);

    // printf("kernelVal = %i\n", kernelVal);

    if (kernelVal > 255)
    {
        kernelVal = 255;
    }

    return kernelVal;
}

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Extract all original RGB values of each pixel and store in an array
    int RGBArrWithPadding[height + 2][width + 2][3];
    int redKernel, greenKernel, blueKernel;

    for (int h = 1; h <= height; h++)
    {
        for (int w = 1; w <= width; w++)
        {
            RGBArrWithPadding[h][w][0] = image[h - 1][w - 1].rgbtRed;
            RGBArrWithPadding[h][w][1] = image[h - 1][w - 1].rgbtGreen;
            RGBArrWithPadding[h][w][2] = image[h - 1][w - 1].rgbtBlue;
        }
    }

    // Black padding around the image
    for (int h = 0; h <= height + 1; h += height + 1)
    {
        for (int w = 0; w <= width + 1; w++)
        {
            RGBArrWithPadding[h][w][0] = 0;
            RGBArrWithPadding[h][w][1] = 0;
            RGBArrWithPadding[h][w][2] = 0;
        }
    }

    for (int w = 0; w <= width + 1; w += width + 1)
    {
        for (int h = 0; h <= height + 1; h++)
        {
            RGBArrWithPadding[h][w][0] = 0;
            RGBArrWithPadding[h][w][1] = 0;
            RGBArrWithPadding[h][w][2] = 0;
        }
    }

    int subRGB[3][3][3]; // Temp RBG Array

    for (int h = 1; h < height; h++)
    {
        for (int w = 1; w < width; w++)
        {
            for (int i = h - 1; i <= h + 1; i++)
            {
                for (int j = w - 1; j <= w + 1; j++)
                {
                    subRGB[i - (h - 1)][j - (w - 1)][0] = RGBArrWithPadding[i][j][0];
                    subRGB[i - (h - 1)][j - (w - 1)][1] = RGBArrWithPadding[i][j][1];
                    subRGB[i - (h - 1)][j - (w - 1)][2] = RGBArrWithPadding[i][j][2];
                    //   printf("subRGB[%i][%i][0] = %i \n", i, j, subRGB[i][j][0]);
                    //   printf("subRGB[%i][%i][1] = %i \n", i, j, subRGB[i][j][1]);
                    //   printf("subRGB[%i][%i][2] = %i \n", i, j, subRGB[i][j][2]);
                }
            }

            redKernel = calc_kernels(subRGB, 0);
            greenKernel = calc_kernels(subRGB, 1);
            blueKernel = calc_kernels(subRGB, 2);

            image[h][w].rgbtRed = blueKernel;
            image[h][w].rgbtGreen = greenKernel;
            image[h][w].rgbtBlue = blueKernel;
        }
    }

    return;
}