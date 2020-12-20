#include<iostream>
#include<vector>
#include<stdio.h>
#include<time.h>

using namespace std;
#define SIZE 4

enum Direction{
    UP, DOWN, LEFT, RIGHT
};

class Tile{
private:
    int value;

public:
    Tile(int x){
        value = x;
    }

    Tile(){
        value = 0;
    }

    void setValue(int x){
        value = x;
    }

    int getValue(){
        return value;
    }

    bool equals(Tile *tile){
        return (value == tile->getValue());
    }

    void merge(Tile *tile){
        setValue(value + tile->getValue());
    }

    void clear(){
        setValue(0);
    }
};

class Grid{
private:
    //Tile *tiles[SIZE][SIZE];

    int tilesum;

    int getNewTileValue(){
        return (rand() % 2 + 1) * 2;
    }

    bool isEmptyTile(vector<Tile *> tileSet){
        for(int i = 0; i < tileSet.size(); i++){
            if(tileSet[i]->getValue() != 0){
                return false;
            }
        }

        return true;
    }

    void slide(vector<Tile *> tileSet){
        slideToEdge(tileSet);
        mergeTile(tileSet);
    }

    void slideToEdge(vector<Tile *> tileSet){
        for(int i = 0; i < tileSet.size(); i++){
            if(remainingIsZero(tileSet, i)){
                return;
            }

            while(tileSet[i]->getValue() == 0){
                slideTo(tileSet, i);
            }
        }
    }

    bool remainingIsZero(vector<Tile *> tileSet, int i){
        vector<Tile *> remainingTile;

        for(int j = i; j < tileSet.size(); j++){
            remainingTile.push_back(tileSet[j]);
        }

        return isEmptyTile(remainingTile);
    }

    void slideTo(vector<Tile *> tileSet, int index){
        for(int j = index; j < tileSet.size() - 1; j++){
            tileSet[j]->setValue(tileSet[j + 1]->getValue());
        }

        tileSet[tileSet.size() - 1]->clear();
    }

    void mergeTile(vector<Tile *> tileSet){
        for(int i = 0; i < tileSet.size() - 1; i++){
            if(tileSet[i]->equals(tileSet[i + 1])){
                tileSet[i]->merge(tileSet[i + 1]);
                tileSet[i + 1]->clear();
                slideTo(tileSet, i + 1);
            }
        }
    }

    bool hasEmptyTile(){
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                if(tiles[i][j]->getValue() == 0){
                    return true;
                }
            }
        }

        return false;
    }

    bool hasEqualNeighbour(){
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                if(j < SIZE - 1){
                    if(tiles[i][j]->equals(tiles[i][j + 1])){
                        return true;
                    }
                }

                if(i < SIZE - 1){
                    if(tiles[i][j]->equals(tiles[i + 1][j])){
                        return true;
                    }
                }
            }
        }

        return false;
    }

public:
    Grid *prev=NULL;
    Tile *tiles[SIZE][SIZE];

    Grid(){
        tilesum=0;
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                tiles[i][j] = new Tile();
            }
        }
    }

    int getTileSum(){
        return this->tilesum;
    }

    void setTileSum(int t){
        this->tilesum=t;
    }

    int getMaxVal(){
        int maxval=0;
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                if(maxval < this->tiles[i][j]->getValue()){
                    maxval= this->tiles[i][j]->getValue();
                }
            }
        }
        return maxval;
    }

    void printGrid(){
        cout << endl;
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                
                cout << tiles[i][j]->getValue() << "\t";
            }
            cout << endl;
        }
        cout<<"tilesum="<<this->tilesum<<"\n";
        if(this->tilesum==8){
            cout<<"You won the game!\n";
            exit(0);
        }
    }

    bool generateNewTile(){
        if(!hasEmptyTile()){
            return false;
        }
        while(true){
            srand(time(0));                 //for new random value
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            if(tiles[x][y]->getValue() == 0){
                int tile_val=getNewTileValue();
                tiles[x][y]->setValue(tile_val);
                tilesum=tilesum+tile_val;
                return true;
            }
        }
    }

    void move(Direction direction){
        for(int i = 0; i < SIZE; i++){
            vector<Tile *> tileSet;

            for(int j = 0; j < SIZE; j++){
                switch(direction){
                    case LEFT:
                        tileSet.push_back(tiles[i][j]);
                        break;
                    case RIGHT:
                        tileSet.push_back(tiles[i][SIZE - j - 1]);
                        break;
                    case UP:
                        tileSet.push_back(tiles[j][i]);
                        break;
                    case DOWN:
                        tileSet.push_back(tiles[SIZE - j - 1][i]);
                        break;
                    default:
                        break;
                }
            }

            if(!isEmptyTile(tileSet)){
                slide(tileSet);
            }
        }
    }

    bool noPossibleMove(){
        if(hasEmptyTile()){
            return false;
        }

        return !hasEqualNeighbour();
    }
    
};

int main(){
    Grid grid;
    Grid *head=&grid;
    Grid *temp;
    

    //generating the initial grid
    for(int i=0;i<2;i++){
        grid.generateNewTile();
    }
    cout<<"The starting grid";
    grid.printGrid();
    cout<<"Maximum tile value="<<grid.getMaxVal()<<"\n";

    while(true){


        srand(time(0));

        Direction rand_dir=(Direction)(rand()%4);

        temp=(Grid *)malloc(sizeof(Grid));
        temp->prev=head;
        temp->setTileSum(head->getTileSum());

        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                temp->tiles[i][j]= (Tile*)malloc(sizeof(Tile));
                temp->tiles[i][j]->setValue(head->tiles[i][j]->getValue());
            }
        }
        head=temp;

        head->move(rand_dir);
        head->generateNewTile();
        if(head->getTileSum()==10){
            cout<<"Discarded state as tilesum=10\n";
            head->printGrid();
            cout<<"\n";
            Grid *del;
            del=head;
            head=head->prev;
            free(del);
            cout<<"Jumping to prev state\n";
        }
        head->printGrid();
        cout<<"Maximum tile value="<<head->getMaxVal()<<"\n";
        
    }
    
    return 0;
}
