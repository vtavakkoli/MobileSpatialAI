#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "mobile_spatial_ai/depth_postprocessor.hpp"
#include "mobile_spatial_ai/frame_scheduler.hpp"
#include "mobile_spatial_ai/semantic_fusion.hpp"
#include "mobile_spatial_ai/spatial_core.hpp"
#include "mobile_spatial_ai/spatial_world.hpp"
namespace { int failures=0; void expect(bool c,const std::string&m){if(!c){++failures;std::cerr<<"FAIL: "<<m<<'\n';}}
void testDepth(){std::vector<float>d(64,2.0F),c(64,.9F);d[27]=50;msai::DepthFrameView v{d.data(),c.data(),8,8,100};msai::Detection2D box{"chair",.95F,0,0,7,7};msai::DepthPostprocessor p;auto r=p.estimateForDetection(v,box);expect(r.valid,"depth valid");expect(std::fabs(r.depth_m-2.0F)<.001F,"median rejects outlier");}
void testFusion(){std::vector<float>d(100,2.0F);msai::DepthFrameView v{d.data(),nullptr,10,10,123};msai::Detection2D box{"door",.8F,4,4,6,6};msai::SemanticFusion f;auto o=f.fuse(box,v,{10,10,5,5},{{1,0,0},{1,0,0,0}});expect(o.label=="door","label survives");expect(std::fabs(o.world_position.x-1.0F)<.01F,"pose x");expect(std::fabs(o.world_position.z-2.0F)<.01F,"depth z");}
void testWorld(){msai::SpatialWorld w(.75F);auto a=w.observe({"chair",.9F,.9F,{}, {1,0,2},1});auto b=w.observe({"chair",.8F,.8F,{}, {1.1F,0,2.1F},2});expect(a==b,"merge same object");expect(w.size()==1,"one object");auto n=w.nearest("chair",{});expect(n.found&&n.object.observations==2,"nearest query");}
void testBehind(){msai::SpatialWorld w;(void)w.observe({"chair",.9F,1,{}, {0,0,-2},1});(void)w.observe({"door",.9F,1,{}, {0,0,2},2});auto o=w.behind(msai::Pose{},10);expect(o.size()==1&&o.front().label=="chair","behind query");}
void testScheduler(){msai::FrameScheduler s;auto lease=s.tryAcquire();auto rejected=s.tryAcquire();expect(static_cast<bool>(lease),"first accepted");expect(!static_cast<bool>(rejected),"stale frame dropped");expect(s.accepted()==1&&s.dropped()==1,"counters exact");}
void testCore(){msai::SpatialCore c;(void)c.observe3D("chair",.9F,{1,0,1},10);expect(c.nearest("chair").found,"core nearest");expect(c.performance().summary("world_update").samples==1,"telemetry recorded");}}
int main(){testDepth();testFusion();testWorld();testBehind();testScheduler();testCore();if(failures){std::cerr<<failures<<" test(s) failed\n";return EXIT_FAILURE;}std::cout<<"All MobileSpatialAI native core tests passed\n";return EXIT_SUCCESS;}
