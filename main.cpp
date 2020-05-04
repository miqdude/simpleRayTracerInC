#include <iostream>
#include <cmath>

struct Vec {
  double x,y,z;
  Vec(){x=y=z=0;}
  Vec(double a, double b, double c){x=a,y=b,z=c;}
  Vec operator + (Vec v) {return Vec(x+v.x,y+v.y,z+v.z);}
  Vec operator - (Vec v) {return Vec(x-v.x,y-v.y,z-v.z);}
  Vec operator * (double d) {return Vec(x*d,y*d,z*d);}
  Vec operator / (double d) {return Vec(x/d,y/d,z/d);}
  Vec normalize() {double mg=sqrt(x*x+y*y+z*z); return Vec(x/mg,y/mg,z/mg);}
};

double dot(Vec v, Vec b) {return (v.x*b.x+v.y*b.y+v.z*b.z);}

struct Ray {
  public:
    Vec o; // Origin
    Vec d; // Direction
    Ray(Vec i, Vec j){o=i,d=j;}
};

struct Sphere {
  Vec c;      // Center
  double r;   // Radius
  Sphere(Vec i, double j){c=i,r=j;}
  bool intersect(Ray ray, double &t){
    Vec o = ray.o;
    Vec d = ray.d;
    Vec oc = o-c;
    double b= 2*dot(oc, d);
    double c= dot(oc,oc)-r*r;
    double disc = b*b-4*c;
    if(disc < 0) return false;
    else {
      disc = sqrt(disc);
      double t0 = -b-disc;
      double t1 = -b+disc;
      t = (t0<t1) ? t0 : t1;
      return true; 
    }
  }
  Vec getNormal(Vec pi){return (pi-c)/r;}
};

struct Color {
  double r,g,b;
  Color(){r=g=b=0;}
  Color(double i, double j, double k){r=i,g=j,b=k;}
  Color operator * (double d) {return Color(r*d,g*d,b*d);}
  Color operator + (Color c) {return Color((r+c.r)/2,(g+c.g)/2,(b+c.b)/2);}
};

int main(){

  // Image size
  const int W = 500; // width
  const int H = 500; // height
  
  std::cout << "P3\n" << W << '\n' << H << '\n' << "255" << std::endl;
  
  Color pixel_col[H][W];

  Color white (255,255,255);
  Color red(255,0,0);
  Sphere sphere (Vec(W/2,H/2,50),50);
  Sphere light (Vec(W/2,0,50),1);

  // looping
  // send ray through each pixel
  for(int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      // Send a ray through each pixel
      Ray ray(Vec(x,y,0), Vec(0,0,1));

      double t = 20000;

      // Check for intersections
      if(sphere.intersect(ray, t)){

        // Point of intersection
        Vec pi = ray.o + ray.d*t;
        
        // Color The pixel
        Vec L = light.c - pi;
        Vec N = sphere.getNormal(pi);
        double dt = dot(L.normalize(), N.normalize());

        pixel_col[y][x] = red + white * dt * 1.0;

        if(pixel_col[y][x].r < 0) pixel_col[y][x].r = 0;
        if(pixel_col[y][x].g < 0) pixel_col[y][x].g = 0;
        if(pixel_col[y][x].b < 0) pixel_col[y][x].b = 0;
        if(pixel_col[y][x].r > 255) pixel_col[y][x].r = 255;
        if(pixel_col[y][x].g > 255) pixel_col[y][x].g = 255;
        if(pixel_col[y][x].b > 255) pixel_col[y][x].b = 255;
      }

      std::cout << (int)pixel_col[y][x].r << std::endl;
      std::cout << (int)pixel_col[y][x].g << std::endl;
      std::cout << (int)pixel_col[y][x].b << std::endl;
    }
  }
  return 0;
}