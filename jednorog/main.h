void DashCheck(int& dashCount, double& dashTime, double& speed, double delta, double& dashCooldown);

void scrollBackground(double& backgroundX, double speed, double delta, SDL_Surface* background);

void menu(bool& menu, int& quit, SDL_Surface* screen, int czarny, int czerwony, int niebieski, char  text[128], char  text2[128], SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer, SDL_Event& event);
