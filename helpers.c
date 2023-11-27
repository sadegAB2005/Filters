#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j -1] = temp;
        }
    }
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int newX = i + dx;
                    int newY = j + dy;

                    if (newX >= 0 && newX < height && newY >= 0 && newY < width)
                    {
                        sumRed += image[newX][newY].rgbtRed;
                        sumGreen += image[newX][newY].rgbtGreen;
                        sumBlue += image[newX][newY].rgbtBlue;
                        count++;
                    }
                }
            }

            temp[i][j].rgbtRed = round((float)sumRed / count);
            temp[i][j].rgbtGreen = round((float)sumGreen / count);
            temp[i][j].rgbtBlue = round((float)sumBlue / count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary table to hold the new values
    RGBTRIPLE new_image[height][width];

    // Define the Sobel operator
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Loop through each pixel in the image
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int sumXRed=0, sumXGreen=0, sumXBlue=0;
            int sumYRed=0, sumYGreen=0, sumYBlue=0;

            // Apply the Sobel operator
            for(int dx=-1; dx<=1; dx++)
            {
                for(int dy=-1; dy<=1; dy++)
                {
                    // Check if the neighbor is inside the image
                    if(i+dx>=0 && i+dx<height && j+dy>=0 && j+dy<width)
                    {
                        // Apply the Sobel operator
                        sumXRed += image[i+dx][j+dy].rgbtRed * Gx[dx+1][dy+1];
                        sumXGreen += image[i+dx][j+dy].rgbtGreen * Gx[dx+1][dy+1];
                        sumXBlue += image[i+dx][j+dy].rgbtBlue * Gx[dx+1][dy+1];

                        sumYRed += image[i+dx][j+dy].rgbtRed * Gy[dx+1][dy+1];
                        sumYGreen += image[i+dx][j+dy].rgbtGreen * Gy[dx+1][dy+1];
                        sumYBlue += image[i+dx][j+dy].rgbtBlue * Gy[dx+1][dy+1];
                    }
                }
            }

            // Calculate the new color values
            int red = round(sqrt(sumXRed * sumXRed + sumYRed * sumYRed));
            int green = round(sqrt(sumXGreen * sumXGreen + sumYGreen * sumYGreen));
            int blue = round(sqrt(sumXBlue * sumXBlue + sumYBlue * sumYBlue));

            // Cap the values at 255
            red = red > 255 ? 255 : red;
            green = green > 255 ? 255 : green;
            blue = blue > 255 ? 255 : blue;

            // Assign the new color values to the pixel
            new_image[i][j].rgbtRed = red;
            new_image[i][j].rgbtGreen = green;
            new_image[i][j].rgbtBlue = blue;
        }
    }

    // Copy the new values to the original image array
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
}
