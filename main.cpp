#include "Login.h"
#include "UserManager.h" 
#include "BookManager.h" 

int main() {

    UserManager userManager;
    BookManager bookManager;


    Login run(userManager, bookManager); 

    run.ShowStartMenu();
    
    return 0;
}