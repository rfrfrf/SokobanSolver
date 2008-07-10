// BoxWorldSolver.cpp : Defines the entry point for the console application.
//

#include "BWSolver.h"
#include <fstream>



void char2tile(char ch, Tile::Enum &t0, Tile::Enum &t1)
{
    t0 = Tile::Empty; t1 = Tile::Empty;

    switch (ch)
    {
    case ' ': t0 = Tile::Empty; t1 = Tile::Empty; break;
    case 'W': t0 = Tile::Empty; t1 = Tile::Wall; break;
    case 'X': t0 = Tile::BoxTarget; t1 = Tile::Empty; break;
    case 'b': t0 = Tile::Empty; t1 = Tile::Box; break;
    case 'B': t0 = Tile::BoxTarget; t1 = Tile::Box; break;
    case 'm': t0 = Tile::Empty; t1 = Tile::Player; break;
    case 'M': t0 = Tile::BoxTarget; t1 = Tile::Player; break;
    }
}

void tile2char(Tile::Enum t0, Tile::Enum t1, char &ch)
{
    ch = ' ';
    if (t0 == Tile::BoxTarget)
    {
        switch(t1)
        {
        case Tile::Empty: ch = 'X';break;
        case Tile::Wall: ch = '?';break;
        case Tile::Box: ch = 'B';break;
        case Tile::Player: ch = 'M';break;
        }
    }
    else
    {
        switch(t1)
        {
        case Tile::Empty: ch = ' ';break;
        case Tile::Wall: ch = 'W';break;
        case Tile::Box: ch = 'b';break;
        case Tile::Player: ch = 'm';break;
        }
    }
}



void readfile(std::string filename, BWSolver & solver)
{
    std::ifstream file(filename.c_str(), std::ios::in);

    int x=0;
    int y=0;
    char ch;
    Tile::Enum t0,t1;

    while (!file.eof())
    {
        file.get(ch);
        char2tile(ch, t0, t1);
        solver.SetTile(0, x, y, t0);
        solver.SetTile(1, x, y, t1);

        x++;
        if (ch==10) 
        {
            x=0;
            y++;
        }
    }

    file.close();
}

void writescreen(BWSolver & solver)
{
    int mx,my;
    solver.GetMapDimensions(mx,my);
    for (int y=0;y<my;y++)
    {
        for (int x=0;x<mx;x++)
        { 
            char ch;
            tile2char(solver.GetTile(0, x, y),
                solver.GetTile(1, x, y),
                ch);
            std::cerr<<ch;
        }
        std::cerr<<std::endl;
    }
}



int main(int argc, char* argv[])
{
    BWSolver solver;

    try
    {
        solver.SetSolutionMaxDepth(15);
        solver.SetMapDimensions(10,10);

        readfile("Default.txt", solver);
        writescreen(solver);

        if (!solver.Solve())
        {
            std::cerr<<"Not solved!"<<std::endl;
        }
        else
        {
            std::cerr<<"Solved!"<<std::endl;
        }

    } 
    catch (BWSolverException ex)
    {
        std::cerr<<"Error:"<<ex.GetErrorMessage().c_str()<<std::endl;
    }


    return 0;
}

