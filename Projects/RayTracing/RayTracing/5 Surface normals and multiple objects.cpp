#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"

// t*t*dot(B,B) + 2*t*dot(A-C,B) + doc(C,C) - R*R = 0


vec3 color(const ray& r, hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	int nx = 600, ny = 300, n = 3;
	char* data = new char[nx * ny * n];

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);

	for (int row = 0; row < ny; row++) {
		for (int col = 0; col < nx; col++) {
			float u = float(col) / float(nx);
			float v = float(ny - row - 1) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 co = color(r, world);
			int ir = int(255.99 * co.r());
			int ig = int(255.99 * co.g());
			int ib = int(255.99 * co.b());

			int index = row * nx * n + col * n;
			data[index] = ir;
			data[index + 1] = ig;
			data[index + 2] = ib;
		}
	}
	stbi_write_jpg("images/5 Surface normals and multiple objects.jpg", nx, ny, n, data, 100);
	system("pause");
}