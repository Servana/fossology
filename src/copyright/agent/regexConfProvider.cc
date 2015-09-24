/*
 * Copyright (C) 2015, Siemens AG
 * Author: Maximilian Huber
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "regexConfProvider.hpp"

using namespace std;

RegexConfProvider* RegexConfProvider::_instance = 0;

bool testIfFileExists(const string& filename)
{
  ifstream f(filename);
  bool exists = f.good();
  f.close();
  return exists;
}

string getRegexConfFile(const string& identity)
{
  string confInSameDir(identity + ".conf");
  
  string confRelativeToTestDir("../../agent/" + identity + ".conf");
  
  string confInInstallDir((sysconfigdir ? string(sysconfigdir) : "/usr/local/share/fossology/")
                          + "/mods-enabled/" + identity +  "/agent/" + identity + ".conf");

  if(testIfFileExists( confInSameDir )){
    return confInSameDir;
  }else if(testIfFileExists( confRelativeToTestDir )){
    return confRelativeToTestDir;
  }else{
    return confInInstallDir;
  }
}

RegexConfProvider::RegexConfProvider(const bool isVerbosityDebug)
{
  glblIsVerbosityDebug = isVerbosityDebug;
}

bool RegexConfProvider::getRegexConfStream(const string& identity,
                                           /*out*/ ifstream& stream)
{
  string confFile = getRegexConfFile(identity);

  if (glblIsVerbosityDebug)
    cout << "try to open conf: " << confFile << endl;
  stream.open(confFile.c_str());

  return stream.is_open();
}

void RegexConfProvider::maybeLoad(const char* identity)
{
  ifstream stream;
  if (getRegexConfStream(identity, stream)) {
    glblRegexMap[identity] = readConfStreamToDict(stream, glblIsVerbosityDebug);
    stream.close();
  } else {
    cout << "cannot open regex definitions in conf: " << getRegexConfFile(identity) << endl;
  }
}

void RegexConfProvider::maybeLoad(const string identity,
                                  istringstream& stream)
{
  glblRegexMap[identity] = readConfStreamToDict(stream, glblIsVerbosityDebug);
}

RegexDict RegexConfProvider::getRegexDict(const string identity)
{
  return glblRegexMap[identity];
}

const char* RegexConfProvider::getRegexValue(const string identity,
                                       const string key)
{
  string returnString(getRegexDict(identity)[key]);
  return returnString.c_str();
}
