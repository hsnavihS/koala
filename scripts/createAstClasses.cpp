#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

const unordered_map<string, vector<string>> EXPR_CLASSES = {
    {"Assign", {"Token name", "Expr value"}},
    {"Binary", {"Expr left", "Token op", "Expr right"}},
    {"Call", {"Expr callee", "Token paren", "vector<Expr*> arguments"}},
    {"Grouping", {"Expr expression"}},
    {"Literal", {"any value"}},
    {"Logical", {"Expr left", "Token op", "Expr right"}},
    {"Unary", {"Token op", "Expr right"}},
    {"Variable", {"Token name"}},
};

const unordered_map<string, vector<string>> STMT_CLASSES = {
    {"Block", {"vector<Stmt*> statements"}},
    {"Expression", {"Expr expression"}},
    {"Function",
     {"Token name", "vector<Token*> parameters", "vector<Stmt*> body"}},
    {"If", {"Expr condition", "Stmt thenBranch", "Stmt elseBranch"}},
    {"Print", {"Expr expression"}},
    {"Var", {"Token name", "Expr initializer"}},
    {"Return", {"Token keyword", "Expr value"}},
    {"While", {"Expr condition", "Stmt body"}},
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

void writeVisitorMethod(ofstream &file, string className, string filename) {
  file << "std::any accept(Visitor &visitor) override {\n";
  file << "  return visitor.visit" << className << filename << "(this);\n";
  file << "}\n\n";
}

void writeExprSpecificImports(ofstream &file) {
  file << "#include \"types/Token.h\"\n" << endl;
}

void writeStmtSpecificImports(ofstream &file) {
  file << "#include \"Expr.h\"\n" << endl;
}

// NOTE: Not very elegant, can be much more memory efficient, but it works
void writeToHeaderFile(ofstream &file,
                       unordered_map<string, vector<string>> &classes,
                       string filename) {
  // header guard and import(s)
  file << "#pragma once\n" << endl;
  file << "#include <any>" << endl;
  file << "#include <vector>\n" << endl;
  file << "#include \"types/Visitor.h\"" << endl;

  // write the specific imports
  if (strcmp(filename.c_str(), "Expr") == 0) {
    writeExprSpecificImports(file);
  } else if (strcmp(filename.c_str(), "Stmt") == 0) {
    writeStmtSpecificImports(file);
  }

  // the base class
  file << "class " << filename << " {" << endl;
  file << "public:\n";
  file << "  virtual std::any accept(Visitor &visitor) = 0;\n" << endl;
  file << "};\n" << endl;

  // the derived classes
  for (auto &astClass : classes) {
    string className = astClass.first;
    vector<string> classMembers = astClass.second;
    vector<pair<string, string>> members = {};
    vector<string> parts;

    // split the class members
    splitClassMembers(classMembers, parts, members);

    // class name
    file << "class " << className << " : public " << filename << " {\n";

    // constructor
    file << "public:\n";
    file << "  " << className << "(";

    // write the constructor parameters
    for (int i = 0; i < members.size(); ++i) {
      file << members[i].first << (className == "Literal" ? " " : " *")
           << members[i].second;
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
    writeVisitorMethod(file, className, filename);

    // define members
    for (auto &member : members) {
      file << member.first << (className == "Literal" ? " " : " *")
           << member.second << ";\n";
    }
    file << "};\n\n";
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " output_file_name" << endl;
    return 1;
  }

  string filename = argv[1];
  string currentPath = filesystem::current_path().string();
  currentPath = currentPath.substr(0, currentPath.rfind("/"));
  string path = currentPath + "/include/types/" + filename + ".h";
  auto classes =
      strcmp(filename.c_str(), "Expr") == 0 ? EXPR_CLASSES : STMT_CLASSES;

  ofstream file(path);

  if (file.is_open()) {
    writeToHeaderFile(file, classes, argv[1]);
    file.close();
  } else {
    cerr << "Error: Could not open file " << path << endl;
    return 1;
  }
}
