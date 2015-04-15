#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
    char format_version[64];
    char user_comments[128];
    char dummy1[8];
    uint32_t image_param_flag;
    uint32_t image_data_type;
    float invalid_point;
    uint32_t array_width;
    uint32_t array_height;
    uint32_t data_block_length;
    uint32_t scale_flag;
    float i_scale;
    float j_scale;
    uint32_t transfo_matrix_flag;
    double transfo_matrix[16];
    uint32_t image_color_flag;
    uint32_t color_block_length;
    uint32_t camera_position_flag;
    float camera_x;
    float camera_y;
    float camera_z;
    uint32_t dummy2[30];
} PIFHEADER;
#pragma pack(pop)

/* Swap bytes for float */
float reverseFloat(const float inFloat)
{
   float retVal;
   char *floatToConvert = (char*)&inFloat;
   char *returnFloat = (char*)&retVal;
   
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}

/* Swap bytes for double */
double reverseDouble(const double inDouble)
{
   double retVal;
   char *doubleToConvert = (char*)&inDouble;
   char *returnDouble = (char*)&retVal;

   returnDouble[0] = doubleToConvert[7];
   returnDouble[1] = doubleToConvert[6];
   returnDouble[2] = doubleToConvert[5];
   returnDouble[3] = doubleToConvert[4];
   returnDouble[4] = doubleToConvert[3];
   returnDouble[5] = doubleToConvert[2];
   returnDouble[6] = doubleToConvert[1];
   returnDouble[7] = doubleToConvert[0];

   return retVal;
}

int main(int argc, char *argv[])
{
    FILE *srcFilePtr;
    FILE *dstFilePtr;
    char *srcFile = argv[1];
    char *dstFile = argv[2];
    int i;
    
    srcFilePtr = fopen(srcFile, "rb");
    if (!srcFilePtr)
    {
        printf("Unable to open file for reading.\n");
        return 1;
    }
    
    dstFilePtr = fopen(dstFile, "w");
    if (!dstFilePtr)
    {
        printf("Unable to open file for writing.\n");
        return 1;
    }
    
    /* Read Block 1 */
    
    PIFHEADER block1;
    fread(&block1, sizeof(PIFHEADER), 1, srcFilePtr);
    
    /* PIF files are big-endian; reverse byte order accordingly */
    /* __builtin_bswap32 is a GCC intrinsic */
    
    block1.image_param_flag = __builtin_bswap32(block1.image_param_flag);
    block1.image_data_type = __builtin_bswap32(block1.image_data_type);
    block1.invalid_point = reverseFloat(block1.invalid_point);
    block1.array_width = __builtin_bswap32(block1.array_width);
    block1.array_height = __builtin_bswap32(block1.array_height);
    block1.data_block_length = __builtin_bswap32(block1.data_block_length);
    block1.scale_flag = __builtin_bswap32(block1.scale_flag);
    block1.i_scale = reverseFloat(block1.i_scale);
    block1.j_scale = reverseFloat(block1.j_scale);
    block1.transfo_matrix_flag = __builtin_bswap32(block1.transfo_matrix_flag);
    for (i = 0; i < 16; i++)
    {
        block1.transfo_matrix[i] = reverseDouble(block1.transfo_matrix[i]);
    }
    block1.image_color_flag = __builtin_bswap32(block1.image_color_flag);
    block1.color_block_length = __builtin_bswap32(block1.color_block_length);
    block1.camera_position_flag = __builtin_bswap32(block1.camera_position_flag);
    block1.camera_x = reverseFloat(block1.camera_x);
    block1.camera_y = reverseFloat(block1.camera_y);
    block1.camera_z = reverseFloat(block1.camera_z);
    
    /* Print header information */
    
    printf("\n");
    printf("format_version        %s\n", block1.format_version);
    printf("user_comments         %s\n", block1.user_comments);
    printf("image_param_flag      %u\n", block1.image_param_flag);
    printf("image_data_type       %u\n", block1.image_data_type);
    printf("invalid_point         %f\n", block1.invalid_point);
    printf("array_width           %u\n", block1.array_width);
    printf("array_height          %u\n", block1.array_height);
    printf("data_block_length     %u\n", block1.data_block_length);
    printf("scale_flag            %u\n", block1.scale_flag);
    printf("i_scale               %u\n", block1.i_scale);
    printf("j_scale               %u\n", block1.j_scale);
    printf("transfo_matrix_flag   %u\n", block1.transfo_matrix_flag);
    printf("transfo_matrix        ");
    for (i = 0; i < 16; i++)
    {
        if (i % 4 == 0)
        {
            printf("\n                      ");
        }
        printf("%.2f ", block1.transfo_matrix[i]);
    }
    printf("\n\n");
    printf("image_color_flag      %u\n", block1.image_color_flag);
    printf("color_block_length    %u\n", block1.color_block_length);
    printf("camera_position_flag  %u\n", block1.camera_position_flag);
    printf("camera_x              %f\n", block1.camera_x);
    printf("camera_y              %f\n", block1.camera_y);
    printf("camera_z              %f\n", block1.camera_z);
    
    /* Read Block 2 */
    
    float x, y, z;
    for (i = 0; i < block1.array_width * block1.array_height; i++)
    {
        fread(&x, sizeof(float), 1, srcFilePtr);
        fread(&y, sizeof(float), 1, srcFilePtr);
        fread(&z, sizeof(float), 1, srcFilePtr);
        x = reverseFloat(x);
        y = reverseFloat(y);
        z = reverseFloat(z);
        fprintf(dstFilePtr, "%f %f %f\n", x, y, z);
    }
    
    fclose(srcFilePtr);
    fclose(dstFilePtr);
    
    return 0;
}



