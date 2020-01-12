int lmotorf = 10;
int lmotorr = 9;
int rmotorf = 11;
int rmotorr = 12;

int trigPin2 = 2;  //front sensor
int echoPin2 = 3;

int trigPin3 = 4;  //right sensor
int echoPin3 = 5;

int trigPin1 = 6;   //left sensor
int echoPin1 = 7;

int wait = 1,i,j,robotDirection;

int matrix[4][4];
int visited[4][4];
int wallMap[4][4];
int rows,columns,sourceX,sourceY,targetX,targetY;
int xPos[4] = {1,-1,0,0};
int yPos[4] = {0,0,1,-1};
int totalSteps;
int currStep = 0;

char route[20];
int currX;
int currY;
char nextRoute;
int currWall;

// Robot Directions
// 0 - The Robot is facing left
// 1 - The Robot is facing up
// 2 - The Robot is facing right
// 3 - The Robot is facing down

// Tells the Robot what step to take next
char tellRoute(int currDirection,char nextCell){
    if(currDirection==0){
        if(nextCell=='W'){
            return 'U';
        }
        else if(nextCell=='N'){
            return 'R';
        }
        else if(nextCell=='S'){
            return 'L';
        }
    }
    else if(currDirection==1){
        if(nextCell=='W'){
            return 'L';
        }
        else if(nextCell=='N'){
            return 'U';
        }
        else if(nextCell=='E'){
            return 'R';
        }
    }
    else if(currDirection==2){
        if(nextCell=='E'){
            return 'U';
        }
        else if(nextCell=='N'){
            return 'L';
        }
        else if(nextCell=='S'){
            return 'R';
        }
    }
    else if(currDirection==3){
        if(nextCell=='S'){
            return 'U';
        }
        else if(nextCell=='E'){
            return 'L';
        }
        else if(nextCell=='W'){
            return 'R';
        }
    }
}

int tellDirection(int currDirection,char nextRoute){
    if(currDirection==0){
        if(nextRoute=='U'){
            return 0;
        }
        else if(nextRoute=='L'){
            return 3;
        }
        else if(nextRoute=='R'){
            return 1;
        }
    }
    else if(currDirection==1){
        if(nextRoute=='U'){
            return 1;
        }
        else if(nextRoute=='L'){
            return 0;
        }
        else if(nextRoute=='R'){
            return 2;
        }
    }
    else if(currDirection==2){
        if(nextRoute=='U'){
            return 2;
        }
        else if(nextRoute=='L'){
            return 1;
        }
        else if(nextRoute=='R'){
            return 3;
        }
    }
    else if(currDirection==3){
        if(nextRoute=='U'){
            return 3;
        }
        else if(nextRoute=='L'){
            return 2;
        }
        else if(nextRoute=='R'){
            return 0;
        }
    }
}

int isValid(int x,int y){
    if(x>=0 && x<rows && y>=0 && y<columns) return 1;
    else return 0;
}

struct int_pair{
    int first;
    int second;
};

struct Queue{
    int front, rear, size;
    unsigned capacity;
    struct int_pair* arr;
};

struct Stack{
    int top;
    unsigned capacity;
    struct int_pair* arr;
};

struct Queue* createQueue(unsigned capacity){
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->arr = (struct int_pair*) malloc(queue->capacity * sizeof(struct int_pair));
    return queue;
}

int isQueueFull(struct Queue* queue){
    return (queue->size == queue->capacity);
}

int isQueueEmpty(struct Queue* queue){
    return (queue->size == 0);
}

void enqueue(struct Queue* queue,struct int_pair item){
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->arr[queue->rear] = item;
    queue->size = queue->size + 1;
}

struct int_pair dequeue(struct Queue* queue){
    struct int_pair item = queue->arr[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

struct int_pair front(struct Queue* queue){
    return queue->arr[queue->front];
}

struct int_pair rear(struct Queue* queue){
    return queue->arr[queue->rear];
}

struct Stack* createStack(unsigned capacity){
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = (struct int_pair*) malloc(stack->capacity * sizeof(struct int_pair));
    return stack;
}

int isStackFull(struct Stack* stack){
    return stack->top == stack->capacity - 1;
}

int isStackEmpty(struct Stack* stack){
    return stack->top == -1;
}

void push(struct Stack* stack, struct int_pair item){
    stack->arr[++stack->top] = item;
}

struct int_pair pop(struct Stack* stack){
    return stack->arr[stack->top--];
}

struct int_pair top(struct Stack* stack){
    return stack->arr[stack->top];
}

void fillInitialMatrix(){
    int currX,currY,i,j,newX,newY;
    struct int_pair currCell;

    for(i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            visited[i][j] = 0;
            matrix[i][j] = 0;
        }
    }

    matrix[targetX][targetY] = 0;

    struct Queue* cellQueue;
    cellQueue = createQueue(20); // Give the size of the Queue based on the size of the mazes

    struct int_pair temp;
    temp.first = targetX;
    temp.second = targetY;

    enqueue(cellQueue,temp);

    while(!isQueueEmpty(cellQueue)){
        currCell = front(cellQueue);
        currX = currCell.first;
        currY = currCell.second;
        dequeue(cellQueue);
        if(visited[currX][currY]) continue;
        visited[currX][currY] = 1;

        for(i=0;i<4;i++){
            newX = currX + xPos[i];
            newY = currY + yPos[i];
            if(isValid(newX,newY) && !visited[newX][newY]){
                matrix[newX][newY] = matrix[currX][currY] + 1;
                temp.first = newX;
                temp.second = newY;
                enqueue(cellQueue,temp);
            }
        }
    }
}

void shortestPath(){
    int currX,currY,currWall,minX,minY;
    struct int_pair currCell;
    int minValue;

    struct Stack* cellStack;
    cellStack = createStack(20);  // Give the size of the Stack based on the size of the mazes

    struct int_pair temp;
    temp.first = sourceX;
    temp.second = sourceY;

    push(cellStack,temp);

    while(!isStackEmpty(cellStack)){
        currCell = top(cellStack);
        currX = currCell.first;
        currY = currCell.second;
        
        pop(cellStack);

        if(currX==targetX && currY==targetY) continue;

        minValue = 1000000;

        currWall = wallMap[currX][currY];

        if(!(currWall & (1<<3))){       // No wall in the north of the current cell
            if(isValid(currX-1,currY)){
                if(minValue>matrix[currX-1][currY]){
                    minValue = matrix[currX-1][currY];
                    minX = currX-1;
                    minY = currY;
                }
            }
        }

        if(!(currWall & (1<<2))){       // No wall in the south of the current cell
            if(isValid(currX+1,currY)){
                if(minValue>matrix[currX+1][currY]){
                    minValue = matrix[currX+1][currY];
                    minX = currX+1;
                    minY = currY;
                }
            }
        }

        if(!(currWall & (1<<1))){       // No wall in the east of the current cell
            if(isValid(currX,currY+1)){
                if(minValue>matrix[currX][currY+1]){
                    minValue = matrix[currX][currY+1];
                    minX = currX;
                    minY = currY+1;
                }
            }
        }

        if(!(currWall & (1<<0))){       // No wall in the west of the current cell
            if(isValid(currX,currY-1)){
                if(minValue>matrix[currX][currY-1]){
                    minValue = matrix[currX][currY-1];
                    minX = currX;
                    minY = currY-1;
                }
            }
        }

        if((matrix[currX][currY]-1)==minValue){
            temp.first = minX;
            temp.second = minY;
            push(cellStack,temp);
        }
        else{
            matrix[currX][currY] = minValue + 1;
            if(!(currWall & (1<<3))){       // No wall in the north of the current cell
                if(isValid(currX-1,currY)){
                    temp.first = currX-1;
                    temp.second = currY;
                    push(cellStack,temp);
                }
            }

            if(!(currWall & (1<<2))){       // No wall in the south of the current cell
                if(isValid(currX+1,currY)){
                    temp.first = currX+1;
                    temp.second = currY;
                    push(cellStack,temp);
                }
            }

            if(!(currWall & (1<<1))){       // No wall in the east of the current cell
                if(isValid(currX,currY+1)){
                    temp.first = currX;
                    temp.second = currY+1;
                    push(cellStack,temp);
                }
            }

            if(!(currWall & (1<<0))){       // No wall in the west of the current cell
                if(isValid(currX,currY-1)){
                    temp.first = currX;
                    temp.second = currY-1;
                    push(cellStack,temp);
                }
            }
        }
    }
}

void setup() {
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  pinMode(lmotorf, OUTPUT);
  pinMode(lmotorr, OUTPUT);
  pinMode(rmotorf, OUTPUT);
  pinMode(rmotorr, OUTPUT);

  rows = 4;
  columns = 4;

  wallMap[0][0] = 11; wallMap[0][1] = 13; wallMap[0][2] = 12; wallMap[0][3] = 10;
  wallMap[1][0] = 5; wallMap[1][1] = 12; wallMap[1][2] = 12; wallMap[1][3] = 2;
  wallMap[2][0] = 13; wallMap[2][1] = 8; wallMap[2][2] = 12; wallMap[2][3] = 6;
  wallMap[3][0] = 13; wallMap[3][1] = 4; wallMap[3][2] = 12; wallMap[3][3] = 14;

  sourceX = 0;
  sourceY = 0;
  targetX = 3;
  targetY = 3;
  robotDirection = 3;

  // Call the functions here to compute the matrix
  fillInitialMatrix();
  delay(1000);

  shortestPath();
  delay(1000);
  
  totalSteps = matrix[sourceX][sourceY];
  currStep = 0;
  currX = sourceX;
  currY = sourceY;

  // Returns the path that the robot should follow from the source to the destination
  while(currX!=targetX || currY!=targetY){
    currWall = wallMap[currX][currY];
    if(!(currWall & (1<<0))){       // No wall in the west of the current cell
      if(isValid(currX,currY-1) && ((matrix[currX][currY-1]+1)==matrix[currX][currY])){
          nextRoute = tellRoute(robotDirection,'W');
          route[currStep++] = nextRoute;
          robotDirection = tellDirection(robotDirection,nextRoute);
          currY = currY-1;
          continue;
      }
    }

    if(!(currWall & (1<<3))){       // No wall in the north of the current cell
      if(isValid(currX-1,currY) && ((matrix[currX-1][currY]+1)==matrix[currX][currY])){
          nextRoute = tellRoute(robotDirection,'N');
          route[currStep++] = nextRoute;
          robotDirection = tellDirection(robotDirection,nextRoute);
          currX = currX-1;
          continue;
      }
    }

    if(!(currWall & (1<<1))){       // No wall in the east of the current cell
      if(isValid(currX,currY+1) && ((matrix[currX][currY+1]+1)==matrix[currX][currY])){
          nextRoute = tellRoute(robotDirection,'E');
          route[currStep++] = nextRoute;
          robotDirection = tellDirection(robotDirection,nextRoute);
          currY = currY+1;
          continue;
      }
    }

    if(!(currWall & (1<<2))){       // No wall in the south of the current cell
      if(isValid(currX+1,currY) && ((matrix[currX+1][currY]+1)==matrix[currX][currY])){
          nextRoute = tellRoute(robotDirection,'S');
          route[currStep++] = nextRoute;
          robotDirection = tellDirection(robotDirection,nextRoute);
          currX = currX+1;
          continue;
      }
    }
  }

  route[totalSteps++] = 'S';
  currStep = 0;
}

int firstsensor(){ // This function is for left sensor
    int duration1, distance1;
    digitalWrite (trigPin1, HIGH);
    delayMicroseconds (10);
    digitalWrite (trigPin1, LOW);
    duration1 = pulseIn (echoPin1, HIGH);
    distance1 = (duration1/2) / 29.1;
    Serial.print("1st Sensor: ");
    Serial.print(distance1);  
    Serial.print("cm    ");
    return distance1;
}

int secondsensor(){ // This function is for front sensor.
    int duration2, distance2;
    digitalWrite (trigPin2, HIGH);
    delayMicroseconds (10);
    digitalWrite (trigPin2, LOW);
    duration2 = pulseIn (echoPin2, HIGH);
    distance2 = (duration2/2) / 29.1;
    Serial.print("2nd Sensor: "); 
    Serial.print(distance2);  
    Serial.print("cm    ");
   return distance2;
}

int thirdsensor(){ // This function is for right sensor.
    int duration3, distance3;
    digitalWrite (trigPin3, HIGH);
    delayMicroseconds (10);
    digitalWrite (trigPin3, LOW);
    duration3 = pulseIn (echoPin3, HIGH);
    distance3 = (duration3/2) / 29.1;
    Serial.print("3rd Sensor: ");   
    Serial.print(distance3);
    Serial.print("cm     ");
   return distance3;  
}

void turnLeft(){
    digitalWrite (lmotorf, LOW);
    digitalWrite (lmotorr, HIGH);
    digitalWrite (rmotorf, HIGH);
    digitalWrite (rmotorr, LOW);
    delay(200);
    //delay(176);
}

void goStraight(){
    digitalWrite (lmotorf, HIGH);
    digitalWrite (lmotorr, LOW);
    digitalWrite (rmotorf, HIGH);
    digitalWrite (rmotorr, LOW);
    // Previous value 573
    delay(600);
}

void turnRight(){
    digitalWrite (lmotorf, HIGH);  
    digitalWrite (lmotorr, LOW);
    digitalWrite (rmotorf, LOW);
    digitalWrite (rmotorr, HIGH);
    //delay(220);
    delay(240);
}

void stop() {
    digitalWrite (lmotorf, LOW);
    digitalWrite (lmotorr, LOW);
    digitalWrite (rmotorf, LOW);
    digitalWrite (rmotorr, LOW);
    delay(500);
}

void loop() {
  if(wait){
    delay(2000);
    wait = 0;
  }
  
  if(route[currStep]=='U'){
    goStraight();
    stop();
    currStep++;
  }
  else if(route[currStep]=='L'){
    turnLeft();
    stop();
    goStraight();
    stop();
    currStep++;
  }
  else if(route[currStep]=='R'){
    turnRight();
    stop();
    goStraight();
    stop();
    currStep++;
  }
  else if(route[currStep]=='S'){
    stop();
  }
}
