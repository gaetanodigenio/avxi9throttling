#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

void Gradient();

// picture resolution
static const int ImageWidth  = 40000;
static const int ImageHeight = 40000;
static const double CxMin    = -2.5;
static const double CxMax    = 1.5;
static const double CyMin    = -2.0;
static const double CyMax    = 2.0;

int main()
{
    double PixelWidth=(CxMax-CxMin)/ImageWidth; /* scaled x coordinate of pixel (must be scaled to lie somewhere in the Mandelbrot X scale (-2.5, 1.5) */
    double PixelHeight=(CyMax-CyMin)/ImageHeight;/* scaled y coordinate of pixel (must be scaled to lie somewhere in the Mandelbrot Y scale (-2.0, 2.0) */

    const int MaxColorComponentValue=255;
    typedef unsigned char pixel_t[3]; // colors [R, G ,B]
    pixel_t *pixels = malloc(sizeof(pixel_t)*ImageHeight*ImageWidth);


    //FILE* fp;

    //initialize_timer ( );
    float t_init = omp_get_wtime();

    #pragma omp parallel shared(pixels)
    {
        int iY;
        #pragma omp for schedule(dynamic)
        for(iY=0; iY<ImageHeight; iY++) {
            double Cy=CyMin + iY*PixelHeight;
            if (fabs(Cy)< PixelHeight/2) {
                Cy=0.0; // Main antenna
            }
            int iX;
            for(iX=0; iX<ImageWidth; iX++) {
                double Cx=CxMin + iX*PixelWidth;
                double Zx=0.0;
                double Zy=0.0;
                double Zx2=Zx*Zx;
                double Zy2=Zy*Zy;
                /* */
                int Iteration;
                const int IterationMax=50;
                const double Bailout=2; // bail-out value
                const double Circle_Radius=Bailout*Bailout; // circle radius

                for (Iteration=0; Iteration<IterationMax && ((Zx2+Zy2)<Circle_Radius); Iteration++) { // 
                    Zy=2*Zx*Zy + Cy;
                    Zx=Zx2-Zy2 + Cx;
                    Zx2=Zx*Zx;
                    Zy2=Zy*Zy;
                };

                if (Iteration==IterationMax) {
                    //  interior of Mandelbrot set = black
                    pixels[iY*ImageWidth + iX][0] = 0;
                    pixels[iY*ImageWidth + iX][1] = 0;
                    pixels[iY*ImageWidth + iX][2] = 0;
                }
                //
                else {
            Gradient((double)((Iteration-log2(log2(sqrt(Zx2+Zy2)))))/IterationMax,pixels[iY*ImageWidth + iX]);
                }
            }
        }
    }

    //fp= fopen("MandelbrotSetNEW.ppm","wb");
    //fprintf(fp,"P6\n %s\n %d\n %d\n %d\n","# no comment",ImageWidth,ImageHeight,MaxColorComponentValue);   
    //fwrite(pixels,sizeof(pixel_t),ImageWidth*ImageWidth,fp);
    //fclose(fp);
    

   //stop_timer ( );
    printf("Total time: %fs\n", omp_get_wtime() - t_init);

    free(pixels);
    return 0;
}

void Gradient(double position,unsigned char c[3])
{


  if (position>1.0){
    if (position-(int)position==0.0) position=1.0;

    else position=position-(int)position;    
  }




  unsigned char nmax=7; // number of color bars 
  double m=nmax*position;

  int n=(int)m; 

  double f=m-n; 
  unsigned char t=(int)(f*255);



switch( n){
   case 0: {
      c[0] =0;
      c[1] = t;
      c[2] = 255;
       break;
    };
   case 1: {
      c[0] = 0;
      c[1] = 255-t;
      c[2] = 255;
       break;
    };
   case 2: {
      c[0] = t;
      c[1] = 255;
      c[2] = 0;
       break;
    };
   case 3: {
      c[0] = 255;
      c[1] = 255 - t;
      c[2] = 0;
       break;
    };
   case 4: {
      c[0] = t;
      c[1] = 0;
      c[2] = 255;
       break;
    };
   case 5: {
      c[0] = 255;
      c[1] = 0;
      c[2] = 255 - t;
       break;
    };
    default: {
      c[0] = 255;
      c[1] = 0;
      c[2] = 0;
       break;
    };

};

};


//gcc-11 -mno-avx -mno-avx2 -mno-avx512f -fopenmp -o mandelbrodt_novect mandelbrodt_set.c -lm

//gcc-11 -mavx -O3 -fopenmp -o mandelbrodt_avx mandelbrodt_set.c -lm 
