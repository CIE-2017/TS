#include <my_global.h>
#include <mysql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

int main(int argc, char **argv)
{
   char *server = "35.240.157.83";
   char *user = "paweemew";
   char *password = "pawee2540"; /* set me first */
   char *database = "game";
   MYSQL *con = mysql_init(NULL);
  
  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con,server, user, password, 
          database, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, "DROP TABLE IF EXISTS pos")) {
      finish_with_error(con);
  }
  
  if (mysql_query(con, "CREATE TABLE pos(Room_Id INT,Game_ID INT, pos_x TEXT, pos_z TEXT)")) {      
      finish_with_error(con);
  }
  
  mysql_close(con);
  exit(0);
}
