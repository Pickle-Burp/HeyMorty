#include "specto.h"

list *new_list(int size)
{
    list *ext = (list*)malloc(sizeof(struct list*));
    ext->next = NULL;
    ext->size = size;
    ext->data = malloc(size * sizeof(double *));
    for(int i=0;i<size;i++)
    {
        ext->data[i]=0;
    }
    return ext;
}

list *create_empty_list(int size,int lon)
{
    if (lon ==0)
    {
        return NULL;
    }
    list *ext = new_list(size);
    list *el = ext;
    for(int i=0;i<lon-1;i++)
    {
        el->next = new_list(size);
        el = el->next;
    }
    return ext;
}

void mod_list_data(list *l,int index,double *data,int size)
{
    list *ext = l;
    int i =0;
    while((i<index)&&(ext!=NULL))
    {
        ext = ext->next;
    }
    if(ext)
    {
        for(i = 0;i<size;i++)
        {
            ext->data[i]=data[i];
        }
    }
}

void apply_fourier_to_list(list *l_in,list *l_out)
{
    list *l1,*l2;
    int N = 320;
    double *in = fftw_alloc_real(N);
    double *out = fftw_alloc_real(N);
    l1 = l_in;
    l2= l_out;
    
    fftw_plan my_plan;
    my_plan =fftw_plan_r2r_1d(320,in,out,FFTW_DHT,FFTW_PATIENT);
    while (l1!=NULL)
    {
        memcpy(in,l1->data,320);
        /**
        fftw_execute(my_plan);
         **/
        memcpy(l2->data,out,320);
        l1 = l1->next;
        l2 = l2->next;
    }
    fftw_destroy_plan(my_plan);
    fftw_free(in);
    fftw_free(out);
}


void free_list(list *l)
{
    list *next;
    while (l!= NULL)
    {
        free(l->data);
        next = l->next;
        free(l);
        l = next;
    }
}

/* Given "bitmap", this returns the pixel of bitmap at the point 
   ("x", "y"). */
static pixel_t * pixel_at (bitmap_t * bitmap, int x, int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
   success, non-zero on error. */
static int save_png_to_file (bitmap_t *bitmap, const char *path)
{
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
    /* "status" contains the return value of this function. At first
       it is set to a value which means 'failure'. When the routine
       has finished its work, it is set to a value which means
       'success'. */
    int status = -1;
    /* The following number is set by trial and error only. I cannot
       see where it it is documented in the libpng manual.
    */
    int pixel_size = 3;
    int depth = 8;
    fp = fopen (path, "wb");
    if (! fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }
    
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }
    
    /* Set up error handling. */

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }
    
    /* Set image attributes. */

    png_set_IHDR (png_ptr,
                  info_ptr,
                  bitmap->width,
                  bitmap->height,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);
    
    /* Initialize rows of PNG. */

    row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
    for (y = 0; y < bitmap->height; y++) {
        png_byte *row = 
            png_malloc (png_ptr, sizeof (uint8_t) * bitmap->width * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->width; x++) {
            pixel_t * pixel = pixel_at (bitmap, x, y);
            *row++ = pixel->red;
            *row++ = pixel->green;
            *row++ = pixel->blue;
        }
    }
    
    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status = 0;
    
    for (y = 0; y < bitmap->height; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);
    
 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
    fclose (fp);
 fopen_failed:
    return status;
}

/* Given "value" and "max", the maximum value which we expect "value"
   to take, this returns an integer between 0 and 255 proportional to
   "value" divided by "max". */
static int pix (double value, int max)
{
    if (value < 0) {
        return 0;
    }
    int ext = (int) (256.0 *(value/(double) max));
    if (ext<0)
    {
        ext=0;
    }
    return ext;
}

void bitmap_chunk_color(bitmap_t *bitmap,int x,int y,int sizex,int sizey,int val)
{
    pixel_t *pixel;
    for(int xe=0;xe<sizex;xe++)
    {
        for(int ye=0;ye<sizey;ye++)
        {
            pixel = pixel_at(bitmap,x+xe,y+ye);
            pixel->blue = val;
            pixel->green = val;
            pixel->red = val;
        }
    }
}

bitmap_t *list_to_bitmap(list *l_out)
{
    bitmap_t *bitmap= (bitmap_t*)malloc(sizeof(bitmap_t*));
    /* Create an image. */

    bitmap->width = 300;
    bitmap->height = 500;

    bitmap->pixels = calloc (bitmap->width * bitmap->height, sizeof (pixel_t));

    if (! bitmap->pixels)
    {
        errx(EXIT_FAILURE,"couldn't allocate memory for bitmap");
    }

    int x =0;
    int val;
    list *l = l_out;
    pixel_t *pixel;
    while (l!= NULL)
    {
        for(int y=0;y<l->size;y++)
        {
            val = pix(l->data[y],200);
            bitmap_chunk_color(bitmap,x*6,bitmap->height-y-1,6,1,val);
        }
        x+=1;
        l=l->next;
    }
    bitmap_chunk_color(bitmap,0,139,bitmap->width,1,255);
    return bitmap;
}



void spectogram(char *pathname)
{
    FILE *fp = fopen(pathname,"r");
    if (!fp) 
    {
        errx(EXIT_FAILURE,"couldn't read from file");
    }
    float receiver[320];
    double darray[320];
    for(int y =0;y<320;y++)
    {
        darray[y]=0;
    }

    list *l_in,*l_out;
    size_t ret = fread(receiver,sizeof(*receiver),320,fp);
    int i =0;
    char *dest = "spectogram .png";
    char num = '1';
    while (ret)
    {
        l_in = create_empty_list(320,50);
        l_out = create_empty_list(320,50);
        while ((i<50) && (ret))
        {
            for(int x=0;x<ret;x++)
            {
                darray[x]=(double)receiver[x];
            }
            mod_list_data(l_in,i,darray,(size_t)ret);
            ret = fread(receiver,sizeof(*receiver),320,fp);
            i+=1;
        }
        apply_fourier_to_list(l_in,l_out);
        //////////////////////////
        /*  list to .png */
        /* Write the image to a file. */
        bitmap_t *bitmap= list_to_bitmap(l_out);
        dest[10]=num;
        if (save_png_to_file (bitmap, dest))
        {
            errx(EXIT_FAILURE,"error while writing png file");
        }
        free(bitmap->pixels);
        free(bitmap);

        free_list(l_in);
        free_list(l_out);
        if(ret==320)
        {
            ret = fread(receiver,sizeof(*receiver),320,fp);
            num+=1;
        }
    }
    
    fclose(fp);
}
