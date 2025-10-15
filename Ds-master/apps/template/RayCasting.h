#ifndef __RayCasting_h
#define __RayCasting_h

#include "geometry/Intersection.h"
#include "graphics/Image.h"
#include "graphics/PrimitiveBVH.h"
#include "graphics/Renderer.h"

namespace cg
{

class RayTracer: public Renderer
{
public:
  static constexpr auto minMinWeight = float(0.001);
  static constexpr auto maxMaxRecursionLevel = uint32_t(1);

  RayTracer(SceneBase&, Camera&);

  auto minWeight() const
  {
    return _minWeight;
  }

  void setMinWeight(float w)
  {
    _minWeight = math::max(w, minMinWeight);
  }

  auto maxRecursionLevel() const
  {
    return _maxRecursionLevel;
  }

  void setMaxRecursionLevel(uint32_t rl)
  {
    _maxRecursionLevel = math::min(rl, maxMaxRecursionLevel);
  }

  void update() override;
  void render() override;
  virtual void renderImage(Image&);

private:
  Reference<PrimitiveBVH> _bvh;
  struct VRC
  {
    vec3f u;
    vec3f v;
    vec3f n;

  } _vrc;
  float _minWeight;
  uint32_t _maxRecursionLevel;
  uint64_t _numberOfRays;
  uint64_t _numberOfHits;
  Ray3f _pixelRay;
  float _Vh;
  float _Vw;
  float _Ih;
  float _Iw;

  void scan(Image& image);
  void setPixelRay(float x, float y);
  Color shoot(float x, float y);
  bool intersect(const Ray3f&, Intersection&);
  Color trace(const Ray3f& ray, uint32_t level, float weight);
  Color shade(const Ray3f&, Intersection&, uint32_t, float);
  bool shadow(const Ray3f&);
  Color background() const;

  vec3f imageToWindow(float x, float y) const
  {
    return _Vw * (x * _Iw - 0.5f) * _vrc.u + _Vh * (y * _Ih - 0.5f) * _vrc.v;
  }

};

}

#endif
