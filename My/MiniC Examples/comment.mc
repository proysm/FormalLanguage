/*
    Mini C has two types of comments: text comment and line comment.
*/

void main()
{ 
  int i;
  
  i = 100;	// i sets 100
  write(i);	// printf it

  char input = 'A';
  switch(input){
    case 'A' :
      printf("a");
      break;
    default :
      printf("NotA");
  }
}
