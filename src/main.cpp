#include "render_world.h"
#include "object.h"
#include <iostream>
#include <unistd.h>
#ifndef NO_OPENGL
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#endif

// Indicates that we are debugging one pixel; can be accessed everywhere.
bool debug_pixel = false;
int width = 0;
int height = 0;
Pixel* pixel_data;
Pixel* data_sol;

void Usage(const char* exec)
{
    std::cerr << "Usage: " << exec << " -i <test-file> [ -s <solution-file> ] [ -o <stats-file> ] [ -x <debug-x-coord> -y <debug-y-coord> ]" << std::endl;
    exit(1);
}
void print_bitmap_string(float x,float y,const std::string& s);
void Parse(Render_World& world, int& width, int& height, const char* test_file);
void Dump_png(Pixel* data, int width, int height, const char* filename);
bool Read_png(Pixel*& data, int& width, int& height, const char* filename);
void Display_Side_By_Side();

int main(int argc, char** argv)
{
    const char* solution_file = 0;
    const char* input_file = 0;
    const char* statistics_file = 0;
    int test_x=-1, test_y=-1;
    bool use_opengl=true;

    // Parse commandline options
    while(1)
    {
    int opt = getopt(argc, argv, "s:i:m:o:x:y:n");
    if (opt == -1) break;
    switch (opt) {
      case 's': solution_file = optarg;
        break;
      case 'n': use_opengl=false;
        break;
      case 'i': input_file = optarg;
        break;
      case 'o': statistics_file = optarg;
        break;
      case 'x': test_x = atoi(optarg);
        break;
      case 'y': test_y = atoi(optarg);
        break;
    }
    }
    if(!input_file) Usage(argv[0]);

    Render_World world;

    // Parse test scene file
    Parse(world,width,height,input_file);

    // Render the image
    world.Render();

    // For debugging.  Render only the pixel specified on the commandline.
    // Useful for printing out information about a single pixel.
    if(test_x>=0 && test_y>=0)
    {
        // Set a global variable to indicate that we are debugging one pixel.
        // This way you can do: if(debug_pixel) cout<<lots<<of<<stuff<<here<<endl;
        debug_pixel = true;

        // Render just the pixel we are debugging
        world.Render_Pixel(ivec2(test_x,test_y));

        // Mark the pixel we are testing green in the output image.
        world.camera.Set_Pixel(ivec2(test_x,test_y),0x00ff00ff);
    }

    // Save the rendered image to disk
    Dump_png(world.camera.colors,width,height,"output.png");

    // If a solution is specified, compare against it.
    data_sol = 0;
    int width2 = 0, height2 = 0;
    std::string input_file_str=input_file, solution_file_target=solution_file?solution_file:"";


    if (!solution_file && use_opengl) solution_file_target=input_file_str.substr(0,input_file_str.find(".txt"))+".png";

    // Read solution from disk
    if (!solution_file_target.empty() && Read_png(data_sol, width2, height2, solution_file_target.c_str())){
        Pixel pixel_data_diff[width*height];
        // for(int i=0;i<width*height;i++) pixel_data_diff[i]=Make_Pixel(0,0,0);
        assert(width == width2);
        assert(height == height2);

        // For each pixel, check to see if it matches solution
        double error = 0, total = 0;
        for (int i = 0; i < height * width; i++) {
            vec3 a = From_Pixel(world.camera.colors[i]);
            vec3 b = From_Pixel(data_sol[i]);
            for (int c = 0; c < 3; c++) {
                double e = fabs(a[c] - b[c]);
                error += e;
                total++;
                b[c] = e;
            }
            pixel_data_diff[i] = Pixel_Color(b);
        }

        // Output information on how well it matches. Optionally save to file
        // to avoid getting confused by debugging print statements.
        FILE* stats_file = stdout;
        if(statistics_file) stats_file = fopen(statistics_file, "w");
        fprintf(stats_file,"diff: %.2f\n",error/total*100);
        if(statistics_file) fclose(stats_file);

        // Output images showing the error that was computed to aid debugging
        Dump_png(pixel_data_diff, width, height, "diff.png");
    }
    // bool display_side_by_side=true;
#ifndef NO_OPENGL
    if(use_opengl){
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
        if(data_sol) glutInitWindowSize(2*width, height);
        else glutInitWindowSize(width, height);
        glutCreateWindow(("Ray Tracing Preview - "+ input_file_str).c_str());
        glClearColor(0,0,0,1);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glShadeModel(GL_SMOOTH);
        glPushAttrib(GL_COLOR_BUFFER_BIT);
        glDrawBuffer(GL_FRONT);
        pixel_data=world.camera.colors;

        Display_Side_By_Side();

        glutDisplayFunc(Display_Side_By_Side);
        glutMainLoop();}
#endif
    delete [] data_sol;

    return 0;
}
#ifndef NO_OPENGL
void Display_Side_By_Side()
{
    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Generate solution with OpenGL

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int pos=0;
    if(data_sol)
    {
        glRasterPos2f(pos,0);
        glDrawPixels(width,height,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,(GLvoid*)data_sol);
        print_bitmap_string(pos,0.95,"Solution");
        pos++;
    }

    glRasterPos2f(pos,0);
    glDrawPixels(width,height,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,(GLvoid*)pixel_data);
    if(data_sol){ print_bitmap_string(pos,0.95,"Yours");}

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glFlush();
}
void print_bitmap_string(float x,float y,const std::string& s)
{
    glColor3f(1,1,0);
    glRasterPos2f(x,y);
    for (unsigned int i =0; i < s.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,s[i]);
}
#endif
