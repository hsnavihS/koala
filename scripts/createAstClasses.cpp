#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

const unordered_map<string, vector<string>> AST_CLASSES = {
    {"Binary", {"Expr left", "Token op", "Expr right"}},
    {"Grouping", {"Expr expression"}},
    {"Literal", {"string value"}},
    {"Unary", {"Token op", "Expr right"}},
};

void split(string s, vector<string> *result, string delimiter) {
  int delimiter_length = delimiter.length();

  while (s.size()) {
    int position = s.find(delimiter);
    result->push_back(s.substr(0, position));
    s = s.substr(position + delimiter_length);
    if (position == string::npos) {
      break;
    }
  }
}

void splitClassMembers(vector<string> classMembers, vector<string> parts,
                       vector<pair<string, string>> &members) {
  for (auto &member : classMembers) {
    split(member, &parts, " ");
    if (parts.size() == 2) {
      members.push_back(make_pair(parts[0], parts[1]));
    } else {
      cerr << "Error: Invalid member format: " << member << endl;
    }
    parts.clear();
  }
}

void writeVisitorMethod(ofstream &file, string className) {
  file << "std::any accept(Visitor &visitor) override {\n";
  file << "  return visitor.visit" << className << "Expr(this);\n";
  file << "}\n\n";
}

// NOTE: Not very elegant, can be much more memory efficient, but it works
void writeToHeaderFile(ofstream &file) {
  // header guard and import(s)
  file << "#pragma once\n" << endl;
  file << "#include <any>\n" << endl;
  file << "#include \"Token.h\"" << endl;
  file << "#include \"Visitor.h\"\n" << endl;

  // the base class
  file << "class Expr {" << endl;
  file << "public:\n";
  file << "  virtual std::any accept(Visitor &visitor) = 0;\n" << endl;
  file << "};\n" << endl;

  // the derived classes
  for (auto &astClass : AST_CLASSES) {
    string className = astClass.first;
    vector<string> classMembers = astClass.second;
    vector<pair<string, string>> members = {};
    vector<string> parts;

    // split the class members
    splitClassMembers(classMembers, parts, members);

    // class name
    file << "class " << className << " : public Expr {\n";

    // constructor
    file << "public:\n";
    file << "  " << className << "(";

    // write the constructor parameters
    for (int i = 0; i < members.size(); ++i) {
      file << members[i].first << " *" << members[i].second;
      if (i < members.size() - 1) {
        file << ", ";
      }
    }

    file << ")\n";
    file << "\t:\n";

    // write the initialization list
    for (int i = 0; i < members.size(); ++i) {
      file << members[i].second << "(" << members[i].second << ")";
      if (i < members.size() - 1) {
        file << ", ";
      }
    }

    file << " {}\n\n";

    // write the visitor method
    writeVisitorMethod(file, className);

    // define members
    file << "private:\n";
    for (auto &member : members) {
      file << member.first << " *" << member.second << ";\n";
    }
    file << "};\n\n";
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " output_file_name" << endl;
    return 1;
  }

  string currentPath = filesystem::current_path().string();
  currentPath = currentPath.substr(0, currentPath.rfind("/"));
  string path = currentPath + "/include/" + argv[1] + ".h";

  ofstream file(path);

  if (file.is_open()) {
    writeToHeaderFile(file);
    file.close();
  } else {
    cerr << "Error: Could not open file " << path << endl;
    return 1;
  }
}
