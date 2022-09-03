/*
Task 1-Codes without repetition of colours 
       Write the following:
       1.A main() function which acts as the codemaker. It initially sets up the hidden code and shows it to us (we can see the code, because the computer will also act as the codebreaker).
       2.In each iteration it invokes the codebreaker, evaluates it’s score in terms of white and black keys pegs, and shows us both the guess as well as the score. A sample output would look 
       like the following:
                    Hidden code: R G B Y 
                    Iteration 1: R W Y G    B-1, W-2 // Guess and score
                    Iteration 2: Y W G O    W-2      // Guess and score
                    Iteration 3: R G K Y    B-3      // Guess and score
                    Iteration 4: R G B Y    B-4
       3.The codebreaker function, codebreaker(), which maintains a set of constraints on the variables and uses a SAT solver to find a new guess that does not have any contradictions with
         the clauses generated from the previous iterations. When it receives the scores (in terms of the white and black key pegs) from the codemaker, it generates suitable clauses and pushes 
         them into the SAT solver.
        
        
        Assumming the code for the diffrent colors-
         Color : Red Green Blue Yellow Orange Purple Violet Silver
         Codes :  1    2    3      4     5      6      7     8
        

        Assuming the code for the diffrent position of diffrent colors-
        
         1  2   3   4   5   6   7   8   9   10   11   12   13   14   15   16
        R1  R2  R3  R4  G1  G2  G3  G4  B1  B2   B3   B4   Y1   Y2   Y3   Y4

        17  18  19  20  21  22  23  24  25  26   27   28   29   30   31   32
        O1  O2  O3  O4  P1  P2  P3  P4  V1  V2   V3   V4   S1   S2   S3   S4

        Here Ai specifies that color A is in ith Position
        For example
            R2---->means Red is in 2nd Position
            O4---->means Orange is in 4th Position
            G3---->means Green is in 3rd position



*/
#include <cstdio>
#include <iostream>
#include <vector>
#include "togasat.hpp"
#include <time.h>
using namespace std;
vector<vector<int>> vec;              //vector of vector for storing set of clauses
bool quickSearch(vector<int>v ,int k)
{
    for(int i=0;i<v.size();i++)
    {
        if((k-1)/4==(v[i]-1)/4)
        {
            return false;
        }
    }
    return true;
}
int searchBlack(string s1, string s2) //search function for counting black pegs
{
    int count, i;
    count = 0;
    for (i = 0; i < s1.size(); i++)
    {
        if (s1[i] == s2[i])
        {
            ++count;
        }
    }
    return count;
}
int searchWhite(string s1, string s2) //search function for counting white pegs
{
    int i, j, count;
    count = 0;
    for (i = 0; i < s2.size(); i++)
    {
        for (j = 0; j < s1.size(); j++)
        {

            if (i != j && s1[i] == s2[j])
            {
                ++count;
            }
        }
    }
    return count;
}
vector<int> findSAT() //function used for solving clauses using togasat solver
{
    togasat::Solver solver;
    for (int i = 0; i < vec.size(); i++)
    {
        solver.addClause(vec[i]);
    }
    solver.solve();
    vector<int> p;
    p.clear();
    int i;
    for (i = 0; i < 32; i++)
    {
        if (solver.assigns[i] == 0)
        {
            p.push_back(i + 1);
        }
    }
    return p;
}
vector<int> decoder(vector<int> p) //convert code for the diffrent position of diffrent colors into thier original color codes
{
    int i, n, pos, code;
    n = p.size();
    vector<int> r(n);
    for (i = 0; i < n; i++)
    {
        pos = p[i] % 4;
        code = p[i] / 4;
        if (pos == 0)
        {
            r[3] = code;
        }
        else
        {
            r[pos - 1] = code + 1;
        }
    }
    return r;
}
string convertCodeToColor(vector<int> t) //convert numeric code to its corressponding color
{
    int i;
    string s;
    for (i = 0; i < 4; i++)
    {
        if (t[i] == 1)
        {
            s = s + 'R';
        }
        else if (t[i] == 2)
        {
            s = s + 'G';
        }
        else if (t[i] == 3)
        {
            s = s + 'B';
        }
        else if (t[i] == 4)
        {
            s = s + 'Y';
        }
        else if (t[i] == 5)
        {
            s = s + 'O';
        }
        else if (t[i] == 6)
        {
            s = s + 'P';
        }
        else if (t[i] == 7)
        {
            s = s + 'V';
        }
        else
        {
            s = s + 'S';
        }
    }
    return s;
}
int main()
{
    togasat::Solver solver;
    vector<int> clause;
    int i, j, l, x, k;
    vector<int> codeBreaker;
    vector<int> decode;

    for (i = 1; i <= 4; i++) //constraint : There should be one color in every position
    {
        clause.push_back(i);
        clause.push_back(i + 4);
        clause.push_back(i + 8);
        clause.push_back(i + 12);
        clause.push_back(i + 16);
        clause.push_back(i + 20);
        clause.push_back(i + 24);
        clause.push_back(i + 28);
        vec.push_back(clause);
        clause.clear();
    }

    for (i = 1; i < 32; i = i + 4) //constraint : One color should be present in one position only
    {
        for (j = i; j < i + 3; j++)
        {
            for (l = j + 1; l < i + 4; l++)
            {
                clause.push_back(-j);
                clause.push_back(-l);
                vec.push_back(clause);
                clause.clear();
            }
        }
    }

    for (i = 1; i <= 28; i++) //constraint : If a color is present in a position then in that position there should not present any another color
    {
        for (j = i + 4; j <= 32; j += 4)
        {
            clause.push_back(-i);
            clause.push_back(-j);
            vec.push_back(clause);
            clause.clear();
        }
    }

    codeBreaker = findSAT();
    srand(time(NULL));

    int p1, p2, p3, p4, k1, k2, k3, k4;
    const int max = 8;
    /* Generating a random 4 digit number from 1 to 8 where each digit is diffrent*/
    do
    {
        p1 = rand() % max + 1;
        p2 = rand() % max + 1;
        p3 = rand() % max + 1;
        p4 = rand() % max + 1;

    } while (p1 == p2 || p1 == p3 || p1 == p4 || p2 == p3 || p2 == p4 || p3 == p4);
    /*Assigning position code for each color code and storing it in codeMaker vector*/
   
    k1 = 4 * (p1 - 1) + 1;
    k2 = 4 * (p2 - 1) + 2;
    k3 = 4 * (p3 - 1) + 3;
    k4 = 4 * (p4 - 1) + 4;

    vector<int> codeMaker;
    codeMaker.push_back(k1);
    codeMaker.push_back(k2);
    codeMaker.push_back(k3);
    codeMaker.push_back(k4);

    decode = decoder(codeBreaker); //calling decode function to get the actual color code in its proper position
    cout << endl;
    for (i = 0; i < decode.size(); i++)
    {
        cout << decode[i] << " ";
    }
    cout << endl;
    int black, white, it, x1, x2;
    it = 1;
    vector<int> temp; //Auxialiary Vector for storing color codes of codemaker
    temp.push_back(p1);
    temp.push_back(p2);
    temp.push_back(p3);
    temp.push_back(p4);

    string codeM, codeB;              //two string variable for storing color for code maker and code breaker respectively
    codeM = convertCodeToColor(temp); //calling convertCodeToColor function

    /*Mastermind Games Start*/

    cout << "Hidden code:  ";
    for (i = 0; i < codeM.size(); i++) //printing the hidden code for Code Maker
    {
        cout << codeM[i] << "   ";
    }
    cout << endl
         << endl
         << endl;
    cout << " GNo "
         << "      Guesses       "
         << " White "
         << " Black \n";
    while (1)
    {
        printf("%3d", it);
        codeB = convertCodeToColor(decode); //calling convertCodeToColor function to convert numerical value to original color code
        black = searchBlack(codeM, codeB);  //return number of black pegs
        white = searchWhite(codeM, codeB);  //return number of white pegs
        for (i = 0; i < codeB.size(); i++)  //printing various guesses made by the codeBreaker
        {
            printf("%5c", codeB[i]);
        }
        decode.clear();

        printf("%6d %6d\n", white, black); //printing white and black pegs
        if (black == 4)
        {
            break;
        }
        ++it;
        //as per the condition we are adding the clauses
        if (black == 0)
        {
            for (i = 0; i < 4; i++)
            {
                clause.push_back(-(codeBreaker[i]));
                vec.push_back(clause);
                clause.clear();
            }
            
            codeBreaker = findSAT();
            decode = decoder(codeBreaker);
        }
        else if (black == 1)
        {
            for (i = 0; i < 3; i++)
            {
                for (j = i + 1; j < 4; j++)
                {
                    clause.push_back(-codeBreaker[i]);
                    clause.push_back(-codeBreaker[j]);
                    vec.push_back(clause);
                    clause.clear();
                }
            }
            for (i = 0; i < 4; i++)
            {
                clause.push_back(codeBreaker[i]);
            }
            vec.push_back(clause);
            
            codeBreaker = findSAT();
            decode = decoder(codeBreaker);
        }
        else if (black == 2)
        {
            for (i = 0; i < 2; i++)
            {
                for (j = i + 1; j < 3; j++)
                {
                    for (k = j + 1; k < 4; k++)
                    {
                        clause.push_back(-codeBreaker[i]);
                        clause.push_back(-codeBreaker[j]);
                        clause.push_back(-codeBreaker[k]);
                        vec.push_back(clause);
                        clause.clear();
                        clause.push_back(codeBreaker[i]);
                        clause.push_back(codeBreaker[j]);
                        clause.push_back(codeBreaker[k]);
                        vec.push_back(clause);
                        clause.clear();
                    }
                }
            }
            
            codeBreaker = findSAT();
            decode = decoder(codeBreaker);
        }
        else if (black == 3)
        {
            for (i = 0; i < 4; i++)
            {
                clause.push_back(-codeBreaker[i]);
            }
            vec.push_back(clause);
            clause.clear();
            for (i = 0; i < 3; i++)
            {
                for (j = i + 1; j < 4; j++)
                {
                    clause.push_back(codeBreaker[i]);
                    clause.push_back(codeBreaker[j]);
                    vec.push_back(clause);
                    clause.clear();
                }
            }
            codeBreaker = findSAT();
            decode = decoder(codeBreaker);
        }
    }
    cout << "Total Number of Iterations:  " << it << endl;
}
