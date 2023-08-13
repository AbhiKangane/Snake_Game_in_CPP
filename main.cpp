#include<iostream>
#include<cstdlib>
#include<conio.h>
#include<windows.h>
#include<ctime>
#include<vector>

using namespace std;

#define width 66
#define height 33


// creating snake class
class Snake{
    private:
        COORD pos;
        int len;
        int velocity;
        char direction;
        vector<COORD> body;
    
    public:
        Snake(COORD pos, int velocity);
        void change_dir(char dir);
        void move_snake();
        COORD get_pos();

        // function for checking food is eaten by snake or not
        bool eaten(COORD food_pos){
            if(food_pos.X == pos.X && food_pos.Y == pos.Y)
                return true;
            return false;
        }

        // for increasing lenth
        void grow(){
            len++;
        }
        
        // check snake collides with wall or not 
        bool collided(){
            // if You want to make GAME OVER when snake head collides with wall of board
            // step 1 : Remove the conditions of Making Transparent wall from move_snake() member function
            // step 2 : Use following condtional statement
            if(pos.X < 1|| pos.Y < 1 || pos.X > width-2 || pos.Y > height-2) 
                return true;

            for(int i=0; i<len-1; i++){
                if(pos.X == body[i].X && pos.Y == body[i].Y) // for checking with sanke head collides with body part or not
                    return true;
            }

            return false;
        }

        // for getting snake body
        vector<COORD> get_Body(){
            return body;
        }
};

Snake :: Snake(COORD pos, int velocity){
    this->pos = pos;
    this->velocity = velocity;
    len = 1;
    direction = 'n';
    body.push_back(pos); //contains snake body, it starting with tail & ending with head
}

void Snake ::change_dir(char dir){
    direction = dir;
} 

void Snake ::move_snake(){
    switch (direction){
        case 'u' : pos.Y -= velocity; break;        
        case 'd' : pos.Y += velocity; break;
        case 'r' : pos.X += velocity; break;
        case 'l' : pos.X -= velocity; break;
    }
    
    // for making transparent walls
    // if(pos.X > width-2)     pos.X = 1;
    // if(pos.X < 1)           pos.X = width - 2;
    // if(pos.Y > height-2)    pos.Y = 1;
    // if(pos.Y < 1)           pos.Y = height - 2;

    body.push_back(pos);

    if(body.size() > len)
        body.erase(body.begin());  // deleting 1st COORD or tail from snake
}

COORD Snake :: get_pos(){
    return pos;
}

// creating sanke object
Snake snake({width/2, height/2}, 1);





// creating food class
class Food{
    private:
    COORD pos;

    public:
    Food(){
        gen_food();
    }
    void gen_food(){
        // generating random position in range[u,l] = rand() % (u-l+1) + 1
        pos.X = rand() % (width-4 + 1) + 1;  // we have to generate food such that it can't overlap by snake
        pos.Y = rand() % (height-4 + 1) + 1;
    }
    COORD get_pos(){
        return pos;
    }
};

// creating food object
Food food;





// creating board
int score;
void board(){
    COORD snake_pos = snake.get_pos();  // for getting snake position
    COORD food_pos = food.get_pos();   // for getting food position
    vector<COORD> snake_Body = snake.get_Body();

    // Printing score
    cout<<"\t\t\t\t\tSCORE : "<<score<<"\n\n";

    // here # is for wall, 0 for snake head, o for snake body part, @ for food
    for (int i = 0; i < height; i++){
        cout<<"\t\t#" ; // for printing 1st col 
        for(int j=0; j<width-2; j++){ // for printing 1st & last row
            if(i==0 || i==height-1)
                cout<<"#";
            
            else if(i==snake_pos.Y && j+1==snake_pos.X)
                cout<<"0";
            
            else if(i==food_pos.Y && j+1==food_pos.X)
                cout<<"@";

            else{
                bool isBodyPart = false;
                for (int k = 0; k < snake_Body.size()-1; k++){
                    if(i == snake_Body[k].Y && j+1 == snake_Body[k].X){
                        cout<<"o";         // printing snake body part
                        isBodyPart = true;
                        break;;
                    }
                }

                if(!isBodyPart)
                    cout<<" ";   // if not body part then print empty spaces
            }
        }
        cout<<"#\n"; // for printing last col
    }   
}





int main(){
    score = 0;
    // cout<<"Click on terminal anywhere to snake game !!!"<<endl;
    srand(time(NULL));

    food.gen_food(); // generating food randomly initially

    bool game_over = false;
    while (!game_over){
        board();

        if(kbhit()){ // if keyboardhit
            switch (getch()){
                case '8' : snake.change_dir('u'); break;
                case '2' : snake.change_dir('d'); break;
                case '4' : snake.change_dir('l'); break;
                case '6' : snake.change_dir('r'); break;
            }
        }

        if(snake.collided()){
            game_over = true;
            cout<<"\n\n\n\t\t\t\t!!!   GAME OVER   !!!\n\t\t\t\t    Your SCORE : "<<score<<endl;
        }

        if(snake.eaten(food.get_pos())){
            food.gen_food();
            snake.grow();
            score += 10;  // per food 10 score
        }

        snake.move_snake();

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});
    }
    

}