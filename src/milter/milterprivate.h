#ifndef __SRC_MILTER_MILTERDATA__
#define __SRC_MILTER_MILTERDATA__

class MilterPrivateData {
 public:
    std::string from;
    std::vector <std::string> to;
    bool xAutoreplyFound = false;
    bool fromMailinglist = false;
};

#endif // __SRC_MILTER_MILTERDATA__
