#include <cctype>
#include <iterator>

#define TYPE_NONE   0
#define TYPE_LI     1
#define TYPE_IM     2
#define TYPE_BO     3
#define TYPE_IT     4
#define TYPE_BP     5
#define TYPE_H1     6
#define TYPE_H2     7
#define TYPE_H3     8
#define TYPE_H4     9
#define TYPE_H5     10
#define TYPE_H6     11
#define TYPE_BQ     12
#define TYPE_BR     13
#define TYPE_CB     14
#define TYPE_UL     15

#define LINK    "li"
#define BOLD    "bo"
#define ITAL    "it"
#define ULIN    "ul"
#define IMAG    "im"
#define POIN    "bp"
#define BLOC    "bq"
#define BREK    "br"
#define HEAD1   "h1"
#define HEAD2   "h2"
#define HEAD3   "h3"
#define HEAD4   "h4"
#define HEAD5   "h5"
#define HEAD6   "h6"
#define CODE    "cb"

#define OPEN_B      '{'
#define CLOSE_B     '}'

#define NO_POS      -1

struct tag_t
{
    uint8_t type = TYPE_NONE;
    int start = NO_POS;
    int end = NO_POS;
    std::string data;
    std::string text;
    bool skip = false;
};

tag_t findTag(int offset, std::string * input);
void findTagStart(std::string &input, std::string::iterator it, tag_t *t);
void findTagEnd(std::string &input, std::string::iterator it, tag_t *t);
void findTagType(std::string &input, tag_t * t);
std::string getReplacementString(tag_t t);

std::string parse(std::string &source) {
    
    bool in_list = false;
    int list_end = 0;
    std::string out = source;
    tag_t t;
    int search_location = 0;
    std::string replaccement;

    do {
        
        t = findTag(search_location, &out);
        


        if (t.type != TYPE_NONE ) {
            
            if (!in_list && t.type == TYPE_BP && t.skip != true) {
                in_list = true;
                out.resize(out.length() + 4);
                out.insert(t.start, "<ul>");
                t.start += 4;
                t.end += 4;
                list_end = t.end;
            } else if (in_list && t.type != TYPE_BP) {
                in_list = false;
                out.resize(out.length() + 5);
                out.insert(list_end, "<\\ul>");
                t.start += 5;
                t.end += 5;
            } else if (in_list && t.type == TYPE_BP) {
                list_end = t.end + 2;
            }


            if (t.skip != true ) {
                search_location = t.start;
                replaccement = getReplacementString(t);
                out.resize(out.length() + replaccement.length() - (t.end - t.start));
                out.replace(t.start, (t.end - t.start) + 1, replaccement);
            } else {
                search_location = t.end - 1;
                out.erase(t.start - 1, 1);
            }

            
        }

    } while(t.type != TYPE_NONE);

    if (in_list) {
        in_list = false;
        out.resize(out.length() + 5);
        out.insert(list_end, "<\\ul>");
        t.start += 5;
        t.end += 5;
    }

    return out;
}

tag_t findTag(int offset, std::string * input) {
    tag_t tag;

    std::string::iterator it = input->begin();
    std::advance(it, offset);
    
    findTagStart(*input, it, &tag);
    if (tag.start == NO_POS) {
        return tag;
    }

    findTagEnd(*input, it, &tag);
    if (tag.end == NO_POS) {
        return tag;
    }

    std::string content = input->substr(tag.start + 2, (tag.end - tag.start) - 3);
    findTagType(content, &tag);

    return tag;

}

void findTagStart(std::string &input, std::string::iterator it, tag_t *t) {
    for (; it != input.end(); ++it ) {
        if ( *it == OPEN_B && *(it + 1) == OPEN_B) {
            t->start = it - input.begin();
            if ( *( it - 1 ) == '\\' ) {
                t->skip = true;
            }
            it++;
            return;
        }
    }
}

void findTagEnd(std::string &input, std::string::iterator it, tag_t *t) {
    for (; it != input.end(); ++it ) {
        if ( *it == CLOSE_B && *(it + 1) == CLOSE_B) {
            it++;
            t->end = it - input.begin();
            return;
        }
    }
}

void findTagType(std::string &input, tag_t * t) {
    
    auto it = input.begin();
    while (isspace(*it)) it++;
    
    int ident_start = it - input.begin();
    while (!isspace(*it)) it++;
    auto iden = input.substr(ident_start, (it - input.begin()) - ident_start);

    if (iden.compare(LINK) == 0) {
        t->type = TYPE_LI;
        while (isspace(*it)) it++;
        int data_start = it - input.begin();
        while (!isspace(*it)) it++;
        t->data = input.substr(data_start, (it - input.begin()) - data_start);
    } else if (iden.compare(IMAG) == 0) {
        t->type = TYPE_IM;
         while (isspace(*it)) it++;
        int data_start = it - input.begin();
        while (!isspace(*it)) it++;
        t->data = input.substr(data_start, (it - input.begin()) - data_start);
    } else if (iden.compare(ITAL) == 0) {
        t->type = TYPE_IT;
    } else if(iden.compare(ULIN) == 0) {
        t->type = TYPE_UL;
    } else if (iden.compare(BOLD) == 0) {
        t->type = TYPE_BO;
    } else if (iden.compare(POIN) == 0) {
        t->type = TYPE_BP;
    } else if (iden.compare(HEAD1) == 0) {
        t->type = TYPE_H1;
    } else if (iden.compare(HEAD2) == 0) {
        t->type = TYPE_H2;
    } else if (iden.compare(HEAD3) == 0) {
        t->type = TYPE_H3;
    } else if (iden.compare(HEAD4) == 0) {
        t->type = TYPE_H4;
    } else if (iden.compare(HEAD5) == 0) {
        t->type = TYPE_H5;
    } else if (iden.compare(HEAD6) == 0) {
        t->type = TYPE_H6;
    } else if (iden.compare(BLOC) == 0){
        t->type = TYPE_BQ;
    } else if (iden.compare(CODE) == 0){
        t->type = TYPE_CB;
    } else if (iden.compare(BREK) == 0) {
        t->type = TYPE_BR;
        return;
    }else {
        t->type = TYPE_NONE;
        return; // No valid tag type so leave
    }

    while (isspace(*it)) it++;
    t->text = input.substr(it - input.begin(), input.length() - (it - input.begin()));
} 

std::string getReplacementString(tag_t t) {
    switch (t.type) {
        case TYPE_LI:
            return "<a href=\"" + t.data + "\">" + t.text + "</a>";
        case TYPE_IM:
            return "<img src=\"" + t.data + "\">" + t.text + "</img>";
        case TYPE_BO:
            return "<b>" + t.text + "</b>";
        case TYPE_IT:
            return "<i>" + t.text + "</i>";
        case TYPE_UL:
            return "<u>" + t.text + "</u>";
        case TYPE_H1:
            return "<h1>" + t.text + "</h1>";
        case TYPE_H2:
            return "<h2>" + t.text + "</h2>";
        case TYPE_H3:
            return "<h3>" + t.text + "</h3>";
        case TYPE_H4:
            return "<h4>" + t.text + "</h4>";
        case TYPE_H5:
            return "<h5>" + t.text + "</h5>";
        case TYPE_H6:
            return "<h6>" + t.text + "</h6>";
        case TYPE_BP:
            return "<li>" + t.text  + "</li>";
        case TYPE_BQ:
            return "<blockquote>" + t.text + "</blockquote>";
        case TYPE_BR:
            return "<br>";
        case TYPE_CB:
            return "<pre><code>" + t.text + "</code></pre>";
    }
    return "***ERR***";
}
