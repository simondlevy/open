#include "framework.hpp"
#include "risp.hpp"

neuro::Processor *neuro::Processor::make()
{
  return new risp::Processor();
}
