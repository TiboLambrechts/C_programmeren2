#include <stdio.h>
#include <stdlib.h>

#define BMPINPUT "test.bmp"
#define BMPOUTPUT "output.bmp"


int main(int argc, char const *argv[])
{
    FILE * inputBMP = fopen(BMPINPUT, "r");
    FILE * outputBMP = fopen(BMPOUTPUT, "w");
    unsigned char header[54] = {0};
    signed int hoogte = 0;
    signed int breedte = 0;
    unsigned char * pixels = NULL;
    unsigned char * pixelsnew = NULL; // worden de nieuwe pixelwaardes in opgeslagen
    int totaalAantalPixels = 0;


    if(inputBMP == NULL)
    {
        printf("%s\n", "ERROR: can't open the file");
        return -1;
    }

    fread(header, 1, 54, inputBMP);

    breedte = header[21] << 24 | header[20] << 16 | header[19] << 8 | header[18];
    printf("De breedte van mijn afbeelding is = %d\n", breedte);
    hoogte = header[25] << 24 | header[24] << 16 | header[23] << 8 | header[22];
    printf("De hoogte van mijn afbeelding is = %d\n", hoogte);

    totaalAantalPixels = breedte * hoogte;
    pixels = (unsigned char *) malloc(totaalAantalPixels*3);

    pixelsnew = pixels; // ervoor zorgen dat de array van nieuwe pixels dezelfde grootte heeft

    if(pixels == NULL)
    {
        printf("ERROR: memory allocation Failed\n");
        return -2;
    }

    fread(pixels, 1, totaalAantalPixels*3, inputBMP);
    printf("INFO: Heap memory allocated = %d (bytes)\n", totaalAantalPixels*3);



    //-------elke pixel waarde weergeven------
    int imageSize = breedte * hoogte * 3;
    for(int i =0; i < imageSize-2; i+=3)
        {
            printf("pixel %d: B= %d, G=%d, R=%d\n", i, pixels[i], pixels[i+1], pixels[i+2]);
        }
    //-----------------------------------------


    //----------onderste rij------------------
    int Hoeveel_Pixels_onderste_rij = (breedte * 3);

    for(int i = 0; i < Hoeveel_Pixels_onderste_rij; i++)
    {
        pixelsnew[i] = pixels[i];
    }
    //----------------------------------------


    //----------bovenste rij ------------------
    int Hoeveel_Pixels_bovenste_rij = (breedte * 3);

    for(int i = ((totaalAantalPixels * 3) - Hoeveel_Pixels_bovenste_rij); i < (totaalAantalPixels * 3); i++)
    {
        pixelsnew[i] = pixels[i];
    }
    //----------------------------------------


    //------------smoothing automatisch-------
    for(int i = (breedte * 3); i < ((totaalAantalPixels * 3) - Hoeveel_Pixels_bovenste_rij); i++)

    {
        pixelsnew[i] = (pixels[i]+pixels[(i+3)]+pixels[(i-3)]+pixels[(i+(breedte * 3))]+pixels[(i+3+(breedte * 3))]+pixels[(i-3)+(breedte * 3)]+pixels[(i-(breedte * 3))]+pixels[(i+3-(breedte * 3))]+pixels[(i-3)-(breedte * 3)])/9;

    }
    //----------------------------------------

/*
    //------------test smoothing hard gecodeerd------------
    signed int Red_gesmooth = 0;
    signed int Blue_gesmooth = 0;
    signed int Green_gesmooth = 0;

    Blue_gesmooth = (pixels[36] + pixels[39] + pixels[42] + pixels[24] + pixels[27] + pixels[30] + pixels[12] + pixels[15] + pixels[18]) / 9;
    printf("De blauw waarde van de gesmoothe pixel = %d\n", Blue_gesmooth);

    Green_gesmooth = (pixels[37] + pixels[40] + pixels[43] + pixels[25] + pixels[28] + pixels[31] + pixels[13] + pixels[16] + pixels[19]) / 9;
    printf("De groen waarde van de gesmoothe pixel = %d\n", Green_gesmooth);

    Red_gesmooth = (pixels[39] + pixels[41] + pixels[44] + pixels[26] + pixels[29] + pixels[32] + pixels[14] + pixels[15] + pixels[20]) / 9;
    printf("De rood waarde van de gesmoothe pixel = %d\n", Red_gesmooth);

    pixelsnew[27] = Blue_gesmooth;
    pixelsnew[28] = Green_gesmooth;
    pixelsnew[29] = Red_gesmooth;
    //----------einde teste smooting---------

*/
    //-----------1 lijst maken van header en nieuwe pixels-----
    int lengte_newWrite = 54 + (totaalAantalPixels * 3);
    unsigned char newWrite[lengte_newWrite];

    for(int i =0; i < 53; i++)
    {
        newWrite[i] = header[i];
    }
    for(int i =0; i < (totaalAantalPixels * 3); i++)
    {
        newWrite[54 + i] = pixelsnew[i];
    }
    //----------------------------------------------------


    //----------------schrijven naar bmp file------------
     fwrite(newWrite, sizeof(newWrite), 1, outputBMP);
    //---------------------------------------------------


    fclose(inputBMP);
    printf("INFO: File %s CLOSED\n", BMPINPUT);
    fclose(outputBMP);
    printf("INFO: File %s CLOSED\n", BMPOUTPUT);

    free(pixels);
    printf("INFO: Heap memory Freed = %d (bytes)\n", totaalAantalPixels*3);
    return 0;
}
