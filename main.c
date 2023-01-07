#define SIMPLE_OBJ_IMP
#include <stdlib.h>
#include <stdio.h>
#include "libs/tinygl-1.0/include/zbuffer.h"
#include <webp/encode.h>
#include <webp/decode.h>
#include <errno.h>
#include "libs/objloader.h"
#include <unistd.h>

/* copyright 2022 thryll (https://brick.cool) */
void loadModelObj(const char *id)
{
    FILE *f;
    char *pathStr = malloc(8 + strlen(id));
    memcpy(pathStr, "models/", 8);
    memcpy(pathStr + 7, id, strlen(id) + 1);


    
    float* verticesX = calloc(3 , sizeof(float));
    float* verticesY = calloc(3 , sizeof(float));
    float* verticesZ = calloc(3 , sizeof(float));

    float* texCoordX = calloc(2, sizeof(float));
    float* texCoordY = calloc(2, sizeof(float));

    unsigned* verticeIndices = malloc(1 * sizeof(unsigned));

    unsigned verticeIndice_i = 0;
    unsigned vertices_i = 0;
    unsigned texCoord_i = 0;

    if ((f = fopen(pathStr, "r")) != NULL)
    {

        if ((fseek(f, 0, SEEK_END) == 0))
        {
            size_t s = ftell(f);
            if (fseek(f, 0, SEEK_SET) == 0)
            {
                SimpleObj_t* obj = loadObj(pathStr);
                drawObj(obj);

                
            }
        }
    }    

    free(pathStr);
}
void renderTexture(const char* id) {
    FILE *f;
    char *pathStr = malloc(10 + strlen(id));
    memcpy(pathStr, "textures/", 10);
    memcpy(pathStr + 9, id, strlen(id) + 1);    
    printf(pathStr);
    if ((f = fopen(pathStr, "r")) != NULL)
    {

        if ((fseek(f, 0, SEEK_END) == 0))
        {
            size_t s = ftell(f);
            char *buffer = malloc(s);
            if (fseek(f, 0, SEEK_SET) == 0)
            {

                fread(buffer, 1, s, f);                
                GLint tex;
                glGenTextures(1, &tex);
                int w;
                int h;
                char* webpDecoded = WebPDecodeRGB(buffer, s, &w, &h);;
                glBindTexture(GL_TEXTURE_2D, tex);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, webpDecoded);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                
                free(buffer);

            }
        }
        fclose(f);
    }    
}
int main(int argc, char **argv)
{
    chdir("/home/renderer");
    const char type = atoi(argv[1]);
    /* this approach is vulnerable to denial of service
    In v2 use a queque system instead */
    const char* username = argv[2];
    /* -1 means no hat to wear 
        currently only a single hat can be utilized
    */
    const char* idHat = argv[3];
    /* -1 means no shirt to wear */
    const char* idShirt = argv[4];
    const char* idPant = argv[5];
    const char* idTool = argv[6];
    const char* idFace = argv[7];




    srand(time(NULL));
    unsigned width = 400;
    unsigned height = 400;
    ZBuffer *zbuffer = ZB_open(width, height, ZB_MODE_RGBA, 0);
    PIXEL *data = malloc(width * height * sizeof(PIXEL));
    glInit(zbuffer);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    



    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lc[] = {1, 1, 1, 1};
    GLfloat lc2[] = {0, 0, 1, 1};
    GLfloat l2p[] = {1, 1, 1, 0};
    GLfloat mat_s[] = { 50.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, l2p);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lc2);

    glLightfv(GL_LIGHT0, GL_SPECULAR, lc);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_s);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lc);
    glEnable(GL_TEXTURE_2D);

    float aspect_ratio = width / height;
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glFrustum(-1, 1, -aspect_ratio, aspect_ratio, 0.5f, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(type == 1)
    glScalef(2, 2, 1);

    if(type == 1)
    glTranslatef(0, -0.8f, -2.5f);
    else if(type == 2) {
    glScalef(4, 4, 1);
    glTranslatef(-0.1f, -1.8f, -2.5f);
    }
    else if(type == 4) {
    glScalef(6, 6, 1);
    glTranslatef(-0.1f, -1.65f, -2.5f);        
    }
    // glRotatef(20, 1, 1, 0);
    glRotatef(20, 0, 1, 0);
    glRotatef(18, 1, 0, 0);
    glRotatef(7, 0, 0, 1);

    if(type == 2 || type == 4 || type == 1) {
    char* modelImage = malloc(strlen(idHat) + 7);
    memcpy(modelImage, idHat, strlen(idHat));
    memcpy(modelImage + strlen(idHat), ".webp", 6);

    
    renderTexture(modelImage);
    char* modelName= malloc(strlen(idHat) + 6);
    memcpy(modelName, idHat, strlen(idHat));
    memcpy(modelName + strlen(idHat), ".obj", 5);
        
    loadModelObj(modelName);
    }

    if(type == 4) {
    if(idFace[0] != '-')
    renderTexture(idFace);
    else
    renderTexture("5.webp");
    loadModelObj("avatar/head.obj");        

    if(idShirt[0] != '-') {
        renderTexture(idShirt);
    }
    else{
        renderTexture("avatar/default.webp");
    }
    loadModelObj("avatar/torso.obj");
    loadModelObj("avatar/right_arm.obj");
    loadModelObj("avatar/left_arm.obj");    
    }
    
    if(type == 1) {
    if(idShirt[0] != '-') {
        renderTexture(idShirt);
    }
    else{
        renderTexture("avatar/default.webp");
    }
    loadModelObj("avatar/torso.obj");
    loadModelObj("avatar/right_arm.obj");
    loadModelObj("avatar/left_arm.obj");
    if(idPant[0] >= '0')
    renderTexture(idPant);
    loadModelObj("avatar/right_leg.obj");
    loadModelObj("avatar/left_leg.obj");
    if(idFace[0] != '-')
    renderTexture(idFace);
    else
    renderTexture("5.webp");
    loadModelObj("avatar/head.obj");
}




    ZB_copyFrameBuffer(zbuffer, data, width * 4);
    char *fixedData = malloc(width * height * sizeof(PIXEL));
    for (unsigned i = 0; i < width * height; i++)
    {
        unsigned char r = GET_RED(data[i]);
        unsigned char g = GET_GREEN(data[i]);
        unsigned char b = GET_BLUE(data[i]);
        fixedData[i * sizeof(PIXEL)] = r;
        fixedData[i * sizeof(PIXEL) + 1] = g;
        fixedData[i * sizeof(PIXEL) + 2] = b;
        fixedData[i * sizeof(PIXEL) + 3] = 255 * (r + g + b > 1);
    }
    unsigned char *webp;
    size_t s = WebPEncodeLosslessRGBA(fixedData, width, height, width * 4, &webp);

    char* renders_avatar = malloc(200);
    memcpy(renders_avatar, "renders/avatar/", 16);
    if(type == 1) renders_avatar  = memcpy(renders_avatar, "renders/avatar/", 16);
    else if(type == 2) renders_avatar  = memcpy(renders_avatar, "renders/item/", 14);
    else if(type == 4) renders_avatar  = memcpy(renders_avatar, "renders/headshots/", 19);
    char* rendersDirectory = malloc(strlen(username) + strlen(renders_avatar) + 7);
    memcpy(rendersDirectory, renders_avatar, strlen(renders_avatar));
    memcpy(rendersDirectory + strlen(renders_avatar), username, strlen(username));
    memcpy(rendersDirectory + strlen(renders_avatar) + strlen(username), ".webp", 6);

    FILE *test = fopen(rendersDirectory, "w+");
    fwrite(webp, s, 1, test);
    fclose(test);

    glClose();
    ZB_close(zbuffer);
    free(webp);
    free(fixedData);
    free(data);
    return 0;
}
