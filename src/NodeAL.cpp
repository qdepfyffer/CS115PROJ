#include "NodeAL.h"

NodeAL::NodeAL(int v, double w) : v(v), w(w), next(nullptr) {}

NodeAL::NodeAL() : NodeAL(0, 0) {}

NodeAL::~NodeAL() {}