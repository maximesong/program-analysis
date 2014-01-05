#include "utils.h"

void DirSettings::setWorkingDir(string dir)
{
  working_dir = dir;
}

string DirSettings::getWorkingDir()
{
  return working_dir;
}

string DirSettings::working_dir = "";
