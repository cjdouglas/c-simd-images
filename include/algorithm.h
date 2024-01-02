#ifndef _SI_ALGORITHM_H
#define _SI_ALGORITHM_H

void apply_brightness(unsigned char strength);
void sse_apply_brightness(unsigned char strength);

void apply_vignette(float radius, float intensity);
void sse_apply_vignette(float radius, float intensity);

#endif  // _SI_ALGORITHM_H
