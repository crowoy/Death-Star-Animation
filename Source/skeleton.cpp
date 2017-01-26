#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"

using namespace std;
using glm::vec3;
using glm::mat3;

/* ----------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                            */

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
vector<vec3> stars (1000);
SDL_Surface* screen;
int t;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update ();
void UpdateStars ();
void Draw ();
void Interpolate (vec3 a, vec3 b, vector<vec3>& result);
void DrawGradient ();
void DrawStars ();

int main (int argc, char* argv[]) {
	screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT );
	t = SDL_GetTicks();	// Set start value for timer.
    
    for (int i = 0; i < stars.size(); i++) {
        // Initialise random star positions
        stars[i].x = float(rand()) / float(RAND_MAX) * 2 - 1; // Generate random number between -1 and 1
        stars[i].y = float(rand()) / float(RAND_MAX) * 2 - 1; // Generate random number between -1 and 1
        stars[i].z = float(rand()) / float(RAND_MAX); // Generate random number between 0 and 1
    }
    
	while (NoQuitMessageSDL()) {
		// Update();
        // Draw();
        
        // Update();
		// DrawGradient();
        
        UpdateStars();
        DrawStars();
	}
    
	SDL_SaveBMP(screen, "screenshot.bmp");

	return 0;
}

void Update () {
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2-t);
	t = t2;
	cout << "Render time: " << dt << " ms." << endl;
}

void UpdateStars () {
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2-t);
	t = t2;
	cout << "Render time: " << dt << " ms." << endl;
    
    for (int i = 0; i < stars.size(); ++i) {
        stars[i].z -= 0.01;
        
        if (stars[i].z <= 0)
            stars[i].z += 1;
            
        if (stars[i].z > 1)
            stars[i].z -= 1;
    }
}

void Draw () {
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			vec3 color(1.0, 1.0, 1.0);
			PutPixelSDL(screen, x, y, color);
		}
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void DrawGradient () {
    vec3 topLeft(1,0,0);        // red
    vec3 topRight(0,0,1);       // blue
    vec3 bottomRight(0,1,0);    // green
    vec3 bottomLeft(1,1,0);     // yellow
    
    vector<vec3> leftSide(SCREEN_HEIGHT);
    vector<vec3> rightSide(SCREEN_HEIGHT);
    
    Interpolate(topLeft, bottomLeft, leftSide);
    Interpolate(topRight, bottomRight, rightSide);
    
    if(SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        vector<vec3> colorRow (SCREEN_WIDTH);
        Interpolate(leftSide[y], rightSide[y], colorRow);
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			PutPixelSDL(screen, x, y, colorRow[x]);
		}
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void DrawStars () {
    SDL_FillRect(screen, 0, 0);
    
    if (SDL_MUSTLOCK(screen))
        SDL_LockSurface(screen);
        
    for (int i = 0; i < stars.size(); ++i) {
        vec3 color = 0.2f * vec3(1,1,1) / (stars[i].z*stars[i].z);
        float x = (SCREEN_HEIGHT/2) * (stars[i].x/stars[i].z) + (SCREEN_WIDTH/2);
        float y = (SCREEN_HEIGHT/2) * (stars[i].y/stars[i].z) + (SCREEN_WIDTH/2);
        
        PutPixelSDL(screen, x, y, color);
    }
    
    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);
        
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Interpolate (vec3 a, vec3 b, vector<vec3>& result) {
    int size = result.size();
    
    if (size == 1) {
        result[0].x = (a.x + b.x) / 2;
        result[0].y = (a.y + b.y) / 2;
        result[0].z = (a.z + b.z) / 2;
    } else {
        result[0].x = a.x;
        result[0].y = a.y;
        result[0].z = a.z;
        
        float stepx = ((b.x - a.x) / (size-1));
        float stepy = ((b.y - a.y) / (size-1));
        float stepz = ((b.z - a.z) / (size-1));
        
        for (float i = 1; i < size; i++){
            result[i].x = result[i-1].x + stepx;
            result[i].y = result[i-1].y + stepy;
            result[i].z = result[i-1].z + stepz;
        }
    }
}