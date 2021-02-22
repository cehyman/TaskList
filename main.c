//A task management app which supports three features: add, list and delete tasks

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct taskitem {
  int taskprio;
  char *taskdesc;
  unsigned int taskid;
  struct taskitem *next;
} taskitem_t;

void addtolist(int, char **);
int cpylist(taskitem_t *);

int main(int argc, char **argv){

  if(*(argv+1) == NULL){ //handles running executable with no commands
    printf("please specify a command, i.e. add, list, delete\n");
    exit(1);
    }

  //add command that adds task to file
  if((strcmp(*(argv+1),"add") == 0) || (strcmp(*(argv+1), "a") == 0)){
     if(*(argv+2) == NULL){//if no priority is entered
      printf("please enter a priority\n");
      exit(1);
    }
    if(*(argv+3) == NULL){//if no task is entered
      printf("please enter a task\n");
      exit(1);
    }
    //add function
    addtolist(argc, argv);
  }

  //outputs list of tasks with highest priority at the lowest point
  if(strcmp(*(argv+1),"list") == 0){
    taskitem_t *listhead = (taskitem_t *) malloc(sizeof(taskitem_t));
    int line = cpylist(listhead); 
    int temprio = 0;
    int tempid = 0;
    char tempdesc[50];
    taskitem_t *node = NULL;
    taskitem_t *temp = NULL;
    //taskitem_t *swap = (taskitem_t *) malloc(sizeof(taskitem_t));
    node = listhead;
    while(node != NULL){//runs until entire list is sorted
      temp = node;
      while(temp->next != NULL){//runs for sort of one element
        if(temp->taskprio > temp->next->taskprio){ //checks if next node is larger than last, if not it switches 
          //moves current to buffer
          temprio = temp->taskprio;
          strcpy(tempdesc, temp->taskdesc);
          tempid = temp->taskid;
          //moves next to current
          temp->taskprio = temp->next->taskprio;
          strcpy(temp->taskdesc, temp->next->taskdesc);
          temp->taskid = temp->next->taskid;
          //moves buffer to next
          temp->next->taskprio = temprio;
          strcpy(temp->next->taskdesc, tempdesc);
          temp->next->taskid = tempid;
        }
        temp = temp->next;
      }
      node = node->next;
    }
    temp = listhead;
    if(line == 1){//prints if only one item is in list
      printf("%d %s %d\n", temp->taskprio, temp->taskdesc, temp->taskid);
    }else{//prints if multiple items in list
      for(int i = 0; i < line; i++){
        temp = temp->next;
        printf("%d %s %d\n", temp->taskprio, temp->taskdesc, temp->taskid);
      }
    }
  }

  //deletes specified task from list
  if(strcmp(*(argv+1),"delete") == 0){
    if(*(argv+2) == NULL){//if no task is entered
      printf("please enter a task ID to delete\n");
      exit(1);
      }

    FILE *fp;
    int rmtask = atoi(*(argv+2)); //task to be deleted
    int line = 0;
    taskitem_t *listhead = (taskitem_t *) malloc(sizeof(taskitem_t));//allocates memory for listhead
    line = cpylist(listhead);//copy list from file function
    fp = fopen("mytask.dat", "w");
    int taskexist = 0;//counter for if the taskexist
    taskitem_t *temp = listhead;
    taskitem_t *previ = NULL;
    if(temp->taskid == rmtask){ //if task to be deleted is in listhead
      listhead = listhead->next;
      free(temp);
      taskexist++;
    }else{ //if task to be deleted is not in listhead
      while(temp->next != NULL && temp->taskid != rmtask){
        previ = temp;
        temp = temp->next;
      }
      if(temp->taskid == rmtask){//removes task
        if(previ){
          previ->next = temp->next;
        }
        free(temp);
        taskexist++;
      }
    }
    if(taskexist == 0){ //if specific task does not exist
      printf("specified task does not exist\n");
      exit(1);
    }else{ //if task exists, print task to file
      temp = listhead;
      for(int i = 0; i < line-1; i++){
        temp = temp->next;
        fprintf(fp, "%d\n%s\n%d\n",temp->taskprio, temp->taskdesc, temp->taskid);
      }
    }
  }
}


void addtolist(int argc, char **argv){
    taskitem_t task;
    FILE *fp;
    int c;
    char buff[4];
    int i = 0;
    int size = 0;
    int line = 0;

    fp = fopen("mytask.dat", "a+");
    fseek(fp, -5, SEEK_END);//looks for last taskid
    while(i != 4){//gets previous taskid
      c = fgetc(fp);
      buff[i] = c;
      i++;
    }
    task.taskprio = atoi(*(argv+2));//changes input to int
    task.taskid = atoi(buff);//changes previous taskid to int
    if(task.taskid >= 1009){//if there have been more than 9 tasks added to list then no more can be added 
      fprintf(stderr, "Task count not be added: too many task\n");
      exit(1);
    }
    task.taskid = task.taskid + 1;//increments tasks id to current task
    if(task.taskid < 1000){//if taskid does not exist then set current taskid to 1000
      task.taskid = 1000;
    }
    for(int i = 3; i < argc; i++){//reads how many words the task is
      size += strlen(*(argv + i)) + 1;
    }
    task.taskdesc = (char *) malloc(size * sizeof(char));//mallocs size of whole task
    for(int i = 3; i < argc; i++){//copys task to list
      strcat(task.taskdesc, *(argv + i));
      strcat(task.taskdesc, " ");
    }
    task.taskdesc[size-1] = '\0';
    fprintf(fp,"%d\n%s\n%d\n", task.taskprio, task.taskdesc, task.taskid);
}


int cpylist(taskitem_t *listhead){
    FILE *fp;
    char buff[4];
    int line = 0;
    int c = 0;
    int j;
    int i = 0;
    taskitem_t *current = listhead;//sets current to listhead
    int test1 = 0;
    char test2[50];
    if((fp = fopen("mytask.dat", "r")) == NULL){//handles if list does not exist
      printf("there are no tasks!\n");
      exit(1);
      }
    while((c = fgetc(fp)) != EOF){//finds how many tasks are in file
      if(c == '\n'){
        line++;
      }
    }
    line = line/3;
    c = 0;
    fseek(fp, 0, SEEK_SET);//resets fp
    while(i < line){
      c = 0;
      j = 0;
      current->next = (taskitem_t *) malloc(sizeof(taskitem_t));
      fscanf(fp, "%d\n", &current->next->taskprio);//reads task priority to list
      while((c = fgetc(fp)) != '\n'){//reads in task to buffer
        test2[j] = c;
        j++;
      }
      test2[j] = '\0';
      fscanf(fp, "%d\n", &test1);//reads taskid to buffer
      current->next->taskdesc = (char *) malloc(strlen(test2) * sizeof(char));//allocates size of task from buffer
      strcpy(current->next->taskdesc, test2);//puts task into list from buffer
      current->next->taskid = test1;//pust task id into list from buffer
      current->next->next = NULL;
      current = current->next;
      i++;
    }
    current = listhead;
    listhead = current->next;
    free(current);
    fclose(fp);
    return line;//returns how many tasks are in file
}
