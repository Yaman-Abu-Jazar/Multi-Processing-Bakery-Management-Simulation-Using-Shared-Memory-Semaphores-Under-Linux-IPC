#include "header.h"
#include "constants.h"
#include "functions.h"

int shmid, shmid_ch, shmid_customer;
pid_t ppid;
SHM memory;
CHM mem_ch;
mem_customer mem_cs;
static int counter1 = 1, counter2 = 2, product;
/*_____________________________________________Drawing helpers___________________________________*/
void drawRectangle(float x1, float y1, float x2, float y2,float r, float g, float b) {
    glColor3f(r,g,b);
    glBegin(GL_QUADS);
      glVertex2f(x1,y1);
       glVertex2f(x2,y1);
      glVertex2f(x2,y2);
       glVertex2f(x1,y2);
    glEnd();
}
void drawCircle(float cx,float cy,float radius,int segments,float r,float g,float b){
    glColor3f(r,g,b);
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(cx, cy);
      for(int i=0;i<=segments;i++){
        float a = 2*PI*i/segments;
        glVertex2f(cx + cosf(a)*radius,
                   cy + sinf(a)*radius);
      }
    glEnd();
}
static void drawText(float x,float y,const char *fmt,...) {
    char buf[128]; va_list ap; va_start(ap,fmt);
    vsnprintf(buf,sizeof(buf),fmt,ap); va_end(ap);
    glRasterPos2f(x,y);
    for(const char *p=buf;*p;p++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*p);
}

static void drawTextFont(void *font, float x, float y, const char *s) {
    glRasterPos2f(x, y);
    for (const char *p = s; *p ; ++p)
        glutBitmapCharacter(font, *p);
}

static void drawCustomer(float x,float y,float bodyR,float bodyG,float bodyB){
    drawRectangle(x-10,y-30,x-2, y, 0.2f,0.2f,0.2f);
    drawRectangle(x+2, y-30,x+10,y, 0.2f,0.2f,0.2f);
    drawRectangle(x-12,y,x+12,y+40,bodyR,bodyG,bodyB);
    drawRectangle(x-30,y+25,x-12,y+30,bodyR,bodyG,bodyB);
    drawRectangle(x+12,y+25,x+30,y+30,bodyR,bodyG,bodyB);
    drawCircle   (x,y+55,15,24,1.0f,0.87f,0.77f);
    drawCircle   (x-5,y+60,2,12,0,0,0);
    drawCircle   (x+5,y+60,2,12,0,0,0);
}

void drawCashier(float x, float y, float bodyR, float bodyG, float bodyB){
    // --- Legs 
    drawRectangle(x-10, y-30, x-2,  y,     0.1f,0.1f,0.1f);
    drawRectangle(x+2,  y-30, x+10, y,     0.1f,0.1f,0.1f);

    // --- Torso (uniform shirt) 
    drawRectangle(x-12, y,    x+12, y+40,  bodyR,bodyG,bodyB);

    // --- Arms 
    drawRectangle(x-30, y+25, x-12, y+30,  bodyR,bodyG,bodyB);
    drawRectangle(x+12, y+25, x+30, y+30,  bodyR,bodyG,bodyB);

    // --- Head ---
    drawCircle   (x, y+55, 15, 24,  1.0f,0.87f,0.77f);

    // --- Eyes ---
    drawCircle   (x-5, y+60, 2, 12,   0.0f,0.0f,0.0f);
    drawCircle   (x+5, y+60, 2, 12,   0.0f,0.0f,0.0f);

    // --- Hat (optional) ---
    drawRectangle(x-20, y+75, x+20, y+85,  0.1f,0.1f,0.1f);  
    drawRectangle(x-15, y+85, x+15, y+100, bodyR,bodyG,bodyB);

    // --- Label the uniform so it’s clear they’re the bakery’s cashier ---
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    drawText(x-70, y+125, "CASHIER Bakery Team");
}

/*________________________________________________ICON DRAWING FUNCTIONS______________________________________________*/ 
void drawPastaIcon(float x, float y)
{
    glColor3f(1.0f, 0.85f, 0.40f);
    glBegin(GL_LINE_STRIP);
    for (float a = 0; a < 2 * M_PI * 3; a += 0.08f) {
        float r = 10 + a;                 
        glVertex2f(x + cosf(a) * r * 0.05f,
                   y + sinf(a) * r * 0.05f);
    }
    glEnd();
}

void drawBreadIcon(float x, float y) {
    // Bread base
    glColor3f(0.93f, 0.76f, 0.45f);
    glBegin(GL_POLYGON);
    for(int i=0; i<=24; ++i) {
        float a = PI * i / 24.0f;
        glVertex2f(x + cosf(a)*10.0f, y + 6.0f + sinf(a)*5.0f);
    }
    glVertex2f(x + 10.0f, y - 8.0f);
    glVertex2f(x - 10.0f, y - 8.0f);
    glEnd();

    // Crust
    glColor3f(0.60f, 0.40f, 0.20f);
    drawRectangle(x-10.0f, y+4.0f, x+10.0f, y+6.0f, 0.60f, 0.40f, 0.20f);
}


/* 🎂 Birthday cake: 2‑layer cake + candle & flame */
void drawCakeIcon(float x, float y)
{
    drawRectangle(x-7,  y-4,  x+7,  y+2,   1.00f, 0.70f, 0.80f);
    drawRectangle(x-5,  y+2,  x+5,  y+6,   0.95f, 0.60f, 0.75f);
    drawRectangle(x-7,  y,    x+7,  y+1,   1.00f, 0.90f, 0.95f);
    drawRectangle(x-1,  y+6,  x+1,  y+11,  0.2f,  0.8f,  0.3f);
    glColor3f(1.0f, 0.8f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x,   y+13);
        glVertex2f(x-2, y+11);
        glVertex2f(x+2, y+11);
    glEnd();
}

void drawSandwichIcon(float x, float y)
{
    // bottom bread slice
    glColor3f(0.90f, 0.80f, 0.60f);
    drawRectangle(x-8, y-4, x+8, y+0,  0.90f,0.80f,0.60f);

    // lettuce
    glColor3f(0.30f, 0.75f, 0.30f);
    drawRectangle(x-8, y-1, x+8, y+2,  0.30f,0.75f,0.30f);

    // tomato slices
    glColor3f(0.85f, 0.15f, 0.15f);
    drawCircle   (x-4, y+1, 2.5f, 16, 0.85f,0.15f,0.15f);
    drawCircle   (x+4, y+1, 2.5f, 16, 0.85f,0.15f,0.15f);

    // top bread slice (rounded)
    glColor3f(0.95f, 0.85f, 0.65f);
    glBegin(GL_POLYGON);
      for(int i=0;i<=16;i++){
        float a = PI * i / 16.0f;
        glVertex2f(x + cosf(a)*8.0f,
                   y + sinf(a)*2.0f + 2.0f);
      }
      glVertex2f(x-8, y+2);
    glEnd();
}

void drawSweetsIcon(float x, float y)
{
    glColor3f(0.95f, 0.75f, 0.35f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for(int i=0;i<=180;i++){
            float a = PI * i / 180.0f;
            glVertex2f(x + cosf(a)*8.0f - 4.0f,
                       y + sinf(a)*4.0f);
        }
        for(int i=180;i>=0;i--){
            float a = PI * i / 180.0f;
            glVertex2f(x + cosf(a)*8.0f + 4.0f,
                       y + sinf(a)*4.0f);
        }
    glEnd();

    glColor3f(0.85f, 0.65f, 0.30f);
    glBegin(GL_POLYGON);
        glVertex2f(x - 4.0f, y);
        glVertex2f(x + 4.0f, y);
        glVertex2f(x + 4.0f, y - 2.0f);
        glVertex2f(x - 4.0f, y - 2.0f);
    glEnd();
}

void drawSavoryPatisserieIcon(float x, float y)
{
    // pastry shell
    glColor3f(0.88f, 0.62f, 0.30f);
    glBegin(GL_POLYGON);
        glVertex2f(x-6.0f, y-4.0f);
        glVertex2f(x+6.0f, y-4.0f);
        glVertex2f(x+4.0f, y+4.0f);
        glVertex2f(x,    y+6.0f);
        glVertex2f(x-4.0f, y+4.0f);
    glEnd();

    // crimped edge
    glColor3f(0.75f, 0.50f, 0.25f);
    glBegin(GL_LINE_STRIP);
        for(int i=0;i<=5;i++){
            float t = i/5.0f;
            float vx = x - 6.0f + t*12.0f;
            float vy = y - 4.0f + sinf(t*PI)*1.5f;
            glVertex2f(vx, vy);
        }
    glEnd();

    // cheese drizzle
    glColor3f(0.95f, 0.90f, 0.10f);
    glBegin(GL_LINES);
        glVertex2f(x-2.0f, y+1.0f);
        glVertex2f(x+2.0f, y+3.0f);
        glVertex2f(x-1.0f, y+3.0f);
        glVertex2f(x+3.0f, y+1.0f);
    glEnd();

    // meat chunks
    glColor3f(0.85f, 0.10f, 0.10f);
    drawRectangle(x-1.0f, y-1.0f, x+1.0f, y+1.0f,  0.85f,0.10f,0.10f);
    drawRectangle(x+2.0f, y+0.5f, x+4.0f, y+2.5f,  0.85f,0.10f,0.10f);
}
void drawSweetPatisserieIcon(float x, float y)
{
    glColor3f(0.25f, 0.10f, 0.05f);
    glBegin(GL_POLYGON);
      for(int i=0;i<32;i++){
        float a = 2*PI*i/32.0f;
        glVertex2f(x + cosf(a)*8.0f,
                   y - 6.0f + sinf(a)*3.0f);
      }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
      // top wavy edge
      for(int i=0;i<=20;i++){
        float t = i/20.0f;
        float xx = x - 8.0f + t*16.0f;
        float yy = y - 2.0f + sinf(t*4*PI)*1.5f;
        glVertex2f(xx, yy);
      }
      // close bottom straight edge
      glVertex2f(x+8.0f, y-4.0f);
      glVertex2f(x-8.0f, y-4.0f);
    glEnd();
    glColor3f(1.0f, 0.75f, 0.25f);
    glBegin(GL_POLYGON);
      for(int i=0;i<=32;i++){
        float a = PI + i*PI/32.0f;
        glVertex2f(x + cosf(a)*8.0f,
                   y + 2.0f + sinf(a)*6.0f);
      }
    glEnd();
    for(int i=-1;i<=1;i++){
      drawCircle(x + i*4.0f, y + 6.0f, 1.2f, 12, 1.0f,1.0f,1.0f);
    }
}
/* ---------------- Price board data (all start at 0) ---------------- */
static const char *productNames[] = {"Bread", "Cakes", "Sandwiches","Sweets", "Sweet pat.", "Savory pat."};
#define N_PRODUCTS (sizeof(productNames)/sizeof(*productNames))
static int productPrice[N_PRODUCTS];

/* --------------- Draw menu / price board (mid‑screen) --------------- */
static void drawPriceBoard(float left, float top)
{
    productPrice[0] = PRICE_OF_BREAD;
    productPrice[1] = PRICE_OF_CAKE;
    productPrice[2] = PRICE_OF_SANDWICH;
    productPrice[3] = PRICE_OF_SWEET;
    productPrice[4] = PRICE_OF_SWEET_PATESSERIES;
    productPrice[5] = PRICE_OF_SAVORY_PATESSERIES;
   const float cellW = 240, cellH = 50;
   const float boardW = cellW + 20;
   const float boardH = N_PRODUCTS * cellH + 40;

   /* board outline */
   drawRectangle(left, top - boardH,
                 left + boardW, top,
                 0.10f, 0.10f, 0.18f);          /* dark background */
   glColor3f(1,1,0);                            /* yellow border  */
   glBegin(GL_LINE_LOOP);
       glVertex2f(left,            top);
       glVertex2f(left+boardW,     top);
       glVertex2f(left+boardW,     top-boardH);
       glVertex2f(left,            top-boardH);
   glEnd();

   /* title */
   glColor3f(1,1,0);
   drawText(left + 40, top - 22, "MENU & PRICES");

   /* rows */
   for(int i=0;i<N_PRODUCTS;i++){
       float cy = top - 40 - i*cellH;

       /* icon – reused from earlier helpers, slightly enlarged */
       glPushMatrix();
           glTranslatef(left + 18, cy, 0);
           glScalef(1.6f, 1.6f, 1.0f);          /* make icons bigger */
           switch(i){
               case 0: drawBreadIcon    (0, 0);         break; 
               case 1: drawCakeIcon       (0,0);        break;
               case 2: drawSandwichIcon   (-2,-2);      break;
               case 3: drawSweetsIcon     (0,0);        break;
               case 4: drawSweetPatisserieIcon(0,0);    break;
               case 5: drawSavoryPatisserieIcon(0,0);   break;
           }
       glPopMatrix();

       /* text */
       glColor3f(1,1,1);
       drawText(left + 50, cy - 6, "%s",        productNames[i]);
       drawText(left + 160, cy - 6, "Price: %d", productPrice[i]);

       /* separator line */
       glColor3f(0.4f,0.4f,0.6f);
       glBegin(GL_LINES);
           glVertex2f(left + 10, cy - cellH/2);
           glVertex2f(left + boardW - 10, cy - cellH/2);
       glEnd();
   }
}

void updateValues(int value) {
    // // Update your values here
    // customerX += 10.0f;  // Move customer right
    // if (customerX > 800) customerX = 100.0f;

    // Request a redraw
    glutPostRedisplay();

    // Call this function again after 1000 ms
    glutTimerFunc(1000, updateValues, 0);
}
//---------------- Animation parameters ------------------------------------- /
static int   W = 1200, H = 750;
static float custX = 60.0f, custY = 400.0f;
static float custX2 = 60.0f, custY2 = 400.0f;
static int   custState = 0, custState2 = 0;
static const float CUST_SPEED = 12000.0f;
static const unsigned int WAIT_MS = 5000;
static unsigned int waitStart = 0;

// Helper returns cashier X/Y & door X/Y given current window size /
static void getRouteTargets(float * cashX, float * cashY){
    float centerX = W - 600;
    float baseY   = 180.0f;
    * cashX = centerX - 75;
    * cashY = baseY   + 35.0f;
}

static void getRouteTargets2(float * cashX, float * cashY){
    float centerX = W - 600;
    float baseY   = 0.0f;
    * cashX = centerX - 75;
    * cashY = baseY   + 35.0f;
}

static void updateCustomer(int v){
    // retrieve target at cashier
    float cashX, cashY;
    getRouteTargets(&cashX, &cashY);

    const float EXIT_Y        = 400.0f;       // y-coordinate to ascend to
    const float OFFSCREEN_X   = W + 60.0f;    // x beyond right edge to hide
    const float dt            = 16.0f / 1000.0f;  // ~60 fps /

    switch(custState){
        case 0:  // walk right to cashier
            custX += CUST_SPEED*dt;
            if(custX >= cashX){ custX = cashX; custState = 1; }
            break;
        case 1:  // walk down to cashier
            custY -= CUST_SPEED * dt;
            if(custY <= cashY) { 
                custY = cashY; custState = 2; waitStart = glutGet(GLUT_ELAPSED_TIME); counter1 += 2; 
            }
            break;
        case 2:  // waiting
            if(glutGet(GLUT_ELAPSED_TIME) - waitStart >= WAIT_MS) custState = 3;
            break;
        case 3:  // ascend, then exit right
            if(custY < EXIT_Y) {
                custY += CUST_SPEED * dt;
                if(custY > EXIT_Y) custY = EXIT_Y;
            } else {
                custX += CUST_SPEED * dt;
                if(custX >= OFFSCREEN_X){
                    custState = 0;
                    custX = 60.0f;
                    custY = 400.0f;
                }
            }
            break;
        default:
            break;
    }

    glutPostRedisplay();
    glutTimerFunc(1000, updateCustomer, 0);
}

static void updateCustomer2(int v){
    // retrieve target at cashier
    float cashX, cashY;
    getRouteTargets2(&cashX, &cashY);

    const float EXIT_Y        = 400.0f;       // y-coordinate to ascend to
    const float OFFSCREEN_X   = W + 60.0f;    // x beyond right edge to hide
    const float dt            = 16.0f / 1000.0f;  // ~60 fps /

    switch(custState2){
        case 0:  // walk right to cashier
            custX2 += CUST_SPEED*dt;
            if(custX2 >= cashX){ custX2 = cashX; custState2 = 1; }
            break;
        case 1:  // walk down to cashier
            custY2 -= CUST_SPEED * dt;
            if(custY2 <= cashY) { 
                custY2 = cashY; custState2 = 2; waitStart = glutGet(GLUT_ELAPSED_TIME); counter2 += 2; 
            }
            break;
        case 2:  // waiting
            if(glutGet(GLUT_ELAPSED_TIME) - waitStart >= WAIT_MS) custState2 = 3;
            break;
        case 3:  // ascend, then exit right
            if(custY2 < EXIT_Y) {
                custY2 += CUST_SPEED * dt;
                if(custY2 > EXIT_Y) custY2 = EXIT_Y;
            } else {
                custX2 += CUST_SPEED * dt;
                if(custX2 >= OFFSCREEN_X){
                    custState2 = 0;
                    custX2 = 60.0f;
                    custY2 = 400.0f;
                }
            }
            break;
        default:
            break;
    }

    glutPostRedisplay();
    glutTimerFunc(1000, updateCustomer2, 0);
}




enum {
    PROD_BREAD = 1,
    PROD_CAKE,
    PROD_SANDWICH,
    PROD_SWEETS,
    PROD_SWEET_PAT,
    PROD_SAVORY_PAT
};
 void drawOrderBoxWithIcon(float cx, float cy, int productID)
 {
     // bubble size
     const float bw = 32.0f, bh = 32.0f;
     // position bubble top-centered above customer’s head
     float bx = cx - bw*0.5f;
     float by = cy + 60.0f;
 
     // 1) bubble fill
     drawRectangle(bx,      by,
                   bx + bw, by + bh,
                   1.0f,1.0f,1.0f);
 
     // 2) bubble border
     glColor3f(0.0f,0.0f,0.0f);
     glBegin(GL_LINE_LOOP);
       glVertex2f(bx,      by);
       glVertex2f(bx + bw, by);
       glVertex2f(bx + bw, by + bh);
       glVertex2f(bx,      by + bh);
     glEnd();
 
    // 3) pointer “tail” filled white
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(cx - 6.0f, by);
    glVertex2f(cx + 6.0f, by);
    glVertex2f(cx,        by - 8.0f);
    glEnd();
    
     // 4) draw the icon centered in the bubble
     glPushMatrix();
       // move to bubble center
    glTranslatef(bx + bw*0.5f,by + bh*0.5f,0.0f);
       // scale down to fit comfortably
       glScalef(0.8f, 0.8f, 1.0f);
       switch (productID) {
         case PROD_BREAD:
             drawBreadIcon       (0,  0);
             break;
         case PROD_CAKE:
             drawCakeIcon       (0,  0);
             break;
         case PROD_SANDWICH:
             drawSandwichIcon   (0,  0);
             break;
         case PROD_SWEETS:
             drawSweetsIcon     (0,  0);
             break;
         case PROD_SWEET_PAT:
             drawSweetPatisserieIcon(0,0);
             break;
         case PROD_SAVORY_PAT:
             drawSavoryPatisserieIcon(0,0);
             break;
       }
     glPopMatrix();
 }

/*___________________________________________________________________________________________________________________________________________*/
static const char *chefTeams[] = {"Prepared Pastes","Prepared Cakes","Prepared Sandwiches","Prepared Sweets","Prepared Sweet Patisseries","Prepared Savory Patisseries"};
#define N_CHEFS (sizeof(chefTeams)/sizeof(*chefTeams))
static const char *bakerTeams[] = {"Baked Cakes", "Baked Sweets", "Bake Sweet Patisseries", "Baked Savory Patisseries","Baked Bread"};
#define N_BAKERS (sizeof(bakerTeams)/sizeof(*bakerTeams))
static const char *stockNames[] = {"Wheat","Yeast","Butter","Milk","Sugar", "Salt","Cheese","Salami", "Sweet items"};
#define N_STOCKS (sizeof(stockNames)/sizeof(*stockNames))

static void display(void) {

    int textOffsetX = 25;
    glClear(GL_COLOR_BUFFER_BIT);

    // Chef Teams & Baker Teams
    int colX[2] = { 20, 20 + 400 };
    glColor3f(1,1,0);
    drawText(colX[0], H-20, "Chef Teams Achievements");
    drawText(colX[1], H-20, "Baker Teams Achievements");
    glColor3f(1,1,1);
    for (int i = 0; i < N_CHEFS; i++) {
        int y = H - 50 - i * 22;
        float iconX = colX[0];      // aligned with left margin
        float iconY = y + 4;        // fine-tuned vertically
        float textX = colX[0] + textOffsetX;
        int number;
    
        switch (i) {
            case 0: drawPastaIcon(iconX, iconY); break;
            case 1: drawCakeIcon(iconX, iconY); break;
            case 2: drawSandwichIcon(iconX - 2, iconY - 2); break;  
            case 3: drawSweetsIcon(iconX, iconY); break;
            case 4: drawSweetPatisserieIcon(iconX, iconY); break;
            case 5: drawSavoryPatisserieIcon(iconX, iconY); break;
        }
    
        glColor3f(1, 1, 1);
        if(i == 0)
            number = mem_ch->prepared_paste;
        else if(i == 1)
            number = mem_ch->prepared_cake;
        else if(i == 2)
            number = memory->sandwiches_ready;
        else if(i == 3)
            number = mem_ch->prepared_sweet;
        else if(i == 4)
            number = mem_ch->prepared_sweet_patisseries;
        else if(i == 5)
            number = mem_ch->prepared_savory_patisseries;
        drawText(textX, y, "%s: %d", chefTeams[i], number);
    }
    
    
    for(int i=0;i<N_BAKERS;i++){
        int y = H-50 - i*22;
        int number;
        if(i == 0)
            number = memory->cakes_ready;
        else if(i == 1)
            number = memory->sweets_ready;
        else if(i == 2)
            number = memory->sweet_patisseries_ready;
        else if(i == 3)
            number = memory->savory_patisseries_ready;
        else if(i == 4)
            number = memory->breads_ready;
        drawText(colX[1], y, "%s: %d", bakerTeams[i], number);
    }

    // shipped border
    glColor3f(1.0f, 1.0f, 0.0f); 
    glBegin(GL_LINES);
    glLineWidth(2.0f); 
    glVertex2f(0,H/2);
    glVertex2f(500,H/2);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(500, 0);  
    glVertex2f(500, H/2);   
    glEnd();

    // Red border
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);  
    glColor3f(1.0f, 0.0f, 0.0f);  
    glBegin(GL_LINES);
    glVertex2f(0,530);
    glVertex2f(860,530);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(860, 530);  
    glVertex2f(860, 860);  

    // Orange
    glColor3f(1.0f, 0.5f, 0.0f); 
    glLineWidth(2.0f); 
    glBegin(GL_LINES);
    glVertex2f(410, 750);  
    glVertex2f(410, 530); 
    glEnd();


    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glLineWidth(2.0f); 
    glVertex2f(860,530);
    glVertex2f(1500,530);
    glEnd();

    // Shipped label above stocks
    glColor3f(1,1,0);
    drawText(40, H/2 - 20, "Shipped");


    // Third section: stats box (top-right)
    int statsX = 870;
    int statsY = H - 30;
    glColor3f(1.0f, 1.0f, 0.0f); 
    drawText(statsX, statsY, "The profit : %d", memory->total_profit);
    drawText(statsX, statsY - 22, "Number frustrated customers : %d", memory->frustrated_customers);
    drawText(statsX, statsY - 44, "Number of customers that complain : %d", memory->complained_customers);
    drawText(statsX, statsY - 66, "Number of customers that requested missing items : %d", memory->missing_items_requests);
    drawText(650, statsY - 210, "Remaining Time : %d", memory->simulation_time_passed--);

    
    // Stock table
    int left = 40, top = H/2 - 40;
    for(int i=0;i<N_STOCKS;i++){
        int row=i/3, col=i%3;
        float x1 = left + col*150;
        float y1 = top - row*70;
        int number;
        if(i == 0)
            number = memory->wheat;
        else if(i == 1)
            number = memory->yeast;
        else if(i == 2)
            number = memory->butter;
        else if(i == 3)
            number = memory->milk;
        else if(i == 4)
            number = memory->sugar;
        else if(i == 5)
            number = memory->salt;
        else if(i == 6)
            number = memory->cheese;
        else if(i == 7)
            number = memory->salami;
        else if(i == 8)
            number = memory->sweet_items;
        drawRectangle(x1,y1,x1+140,y1-60,0.15f,0.15f,0.25f);
        float cx = x1+20, cy=y1-30;
        drawCircle(cx,cy,12,20,0.8f,0.4f,0.0f);
        glColor3f(1,1,1);
        drawText(cx+20, cy+5, "%s", stockNames[i]);
        drawText(cx+20, cy-12, "Qty: %d", number);
    }

    // Truck 
    {   float tx = 480, ty = 30;
        // main body
        drawRectangle(tx, ty,tx - 200, ty + 40,0.2f, 0.2f, 0.6f);
        drawRectangle(tx - 200, ty + 40,tx - 60,  ty + 80,0.2f, 0.4f, 0.8f);
        // wheels
        drawCircle(tx - 40,  ty, 15, 20,0.1f, 0.1f, 0.1f);
        drawCircle(tx - 160, ty, 15, 20,0.1f, 0.1f, 0.1f);}


    // ------------------ Cashier Zone (Bottom-Right, Enlarged) -------------------
    float centerX = W - 600;        
    float baseY   = 180;

    drawCashier(centerX + 75, baseY + 35, 0.3f, 0.9f, 0.3f);

    /* desk ... unchanged ... */
   /* Cashier & desk (same visuals as original, only factored out) */
    drawCashier(centerX + 75, baseY + 35, 0.3f,0.9f,0.3f);
    drawRectangle(centerX-40, baseY+30, centerX+40, baseY+55, 0.5f,0.3f,0.1f);
    drawRectangle(centerX-25, baseY+55, centerX-10, baseY+65, 0.9f,0.9f,0.0f);
    drawRectangle(centerX-40, baseY+15, centerX+40, baseY+55, 0.5f,0.3f,0.1f);
    drawRectangle(centerX-40, baseY+5,  centerX+40, baseY+55, 0.5f,0.3f,0.1f);
    glColor3f(1,0.8,0.1);
     drawText(centerX-10, baseY+38, "Cash 1");

    // second cashier position
    float cash2X = centerX;
    float cash2Y = baseY -145;

    drawCashier(centerX + 75, baseY - 145, 0.3f,0.9f,0.3f);
    drawRectangle(cash2X - 40, cash2Y -  5,cash2X + 40, cash2Y + 20,0.5f, 0.3f, 0.1f);
    drawRectangle(cash2X - 25, cash2Y + 20,cash2X - 10, cash2Y + 30,0.9f, 0.9f, 0.0f);
    drawRectangle(cash2X - 40, cash2Y - 20,cash2X + 40, cash2Y + 20,0.5f, 0.3f, 0.1f);
    drawRectangle(cash2X - 40, cash2Y - 30,cash2X + 40, cash2Y + 20,0.5f, 0.3f, 0.1f);
    glColor3f(1, 0.8f, 0.1f);
    drawText(cash2X - 10, cash2Y + 3, "Cash 2");

    /* Customer (only while state < 4) */
    if(custState < 4) drawCustomer(custX, custY, 0.2f,0.6f,0.95f);
    if(custState==2){
        product = mem_cs->c[counter1].order;
        drawOrderBoxWithIcon(centerX-75, 60, product);
    }

    if(custState2 < 4) drawCustomer(custX2, custY2, 0.2f,0.6f,0.95f);
    if(custState==2){
        product = mem_cs->c[counter2].order;
        drawOrderBoxWithIcon(centerX-75, baseY+60, product);
    }
     

    /* ------------------ NEW PRICE BOARD -------------------------------- */
    drawPriceBoard(1050, 335);      
    glColor3f(0.0f, 1.0f, 0.0f);
    drawTextFont(GLUT_BITMAP_HELVETICA_18, 1150, 460, "Exit");


    glutSwapBuffers();
}

static void reshape(int w,int h){ W=w; H=h; glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); 
}

int main(int argc,char**argv){
    glutInit(&argc,argv);
    ppid = getppid();
    if((shmid = shmget((key_t)ppid, sizeof(SHM), IPC_CREAT | 0777)) != -1){
        if((memory = (SHM)shmat(shmid, NULL, 0)) == (SHM) -1){
            perror("Attachment Shared Memory -- Drawer -- Error.\n");
            exit(9);
        }
    } else {
        perror("Creation Shared Memory -- Drawer -- Error.\n");
        exit(9);
    }

    if((shmid_ch = shmget(CHEF_MEM_KEY, sizeof(CHM), IPC_CREAT | 0777)) != -1){
        if((mem_ch = (CHM)shmat(shmid_ch, NULL, 0)) == (CHM) -1){
            perror("Attachment Chef Shared Memory -- Drawer -- Error.\n");
            exit(9);
        }
    } else {
        perror("Creation Chef Shared Memory -- Drawer -- Error.\n");
        exit(9);
    }

    if((shmid_customer = shmget(CUSTOMER_SHM_KEY, sizeof(mem_customer), 0777)) != -1){
        if((mem_cs = (mem_customer)shmat(shmid_customer, NULL, 0)) == (mem_customer)-1){
            perror("Shared Memory Attachment -- Chef -- Error.\n");
            exit(1);
        }
    } else {
        perror("Shared Memory Creation Error.\n");
        exit(1);
    }
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(W,H);
    glutCreateWindow("Bakery Drawer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glClearColor(0.05f,0.05f,0.08f,1);

    glutTimerFunc(1000, updateValues, 0);  // Start timer
    glutTimerFunc(1000,updateCustomer,0);
    glutTimerFunc(1000,updateCustomer2,0);
    glutMainLoop();
    return 0;
}