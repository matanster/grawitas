#include <ctime>
#include <iostream>

#include "../models/comment.h"

bool operator==(const std::tm t1, std::tm t2);

std::ostream& operator<<(std::ostream& ostr, const Grawitas::Comment& c);
std::ostream& operator<<(std::ostream& ostr, const std::tm& d);
