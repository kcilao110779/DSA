// #include <string>
// #include <vector>
// #include <fstream>
// using namespace std;

// int main()
// {
//     ifstream in("input.txt");
//     string inputStr;
//     vector<string> inputContent;
//     while (getline(in, inputStr))
//     {
//         inputContent.push_back(inputStr);
//     }
//     in.close();

//     ofstream out("output.txt");
//     for (int i = 0; i < inputContent.size(); i++)
//     {
//         out << inputContent[i] << endl;
//     }
//     out.close();

//     return 0;
// }

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
    FILE *fp = fopen("input.txt", "r");
    char line[80];
    while (fgets(line, sizeof(line), fp))
    {
        printf("%s", line);
    }
    fclose(fp);
    system("pause");
}