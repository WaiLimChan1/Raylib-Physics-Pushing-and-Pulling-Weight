#include"raylib.h"
#include<string>
#include<vector>
using namespace std;

//Instructions
//-------------------------------------------------------
// Click right mouse button to toggle between 4 different elements to change
// You can change the left force, right force, kinetic friction ratio, and static friction ratio
// Press W to increase angle or increase ratio
// Press S to decrease angle or decrease ratio
// Press D to increase force magnitude
// Press A to decrease force magnitude
// 
// Press R to reset
// 
//-------------------------------------------------------

//Global Constant Variables
//-------------------------------------------------------
const float SCREEN_WIDTH = 1500;
const float SCREEN_HEIGHT = 900;

enum toggledElement {leftForce, rightForce, kineticFriction, staticFriction};
enum ForceName {LeftForce, RightForce, ForceOfFriction, ForceDueToGravity, NormalForce};

const float INITIAL_POS_X = GetScreenWidth() / 2;
const float INITIAL_POS_Y = GetScreenHeight() / 2;

const float WIDTH = 300;
const float HEIGHT = 100;
const float MASS = 100;

const float GRAVITY = 10; //Force due to Gravity

const float forceMaxCap = 50;
const float forceMinCap = 0;

const float collisionLost = 0.4;
//-------------------------------------------------------

class Brick {
private:
	float posX;
	float posY;
	float velX;
	float velY;
	float accX;
	float accY;

	float width;
	float height;
	float mass;

	float horizontalForce;
	float verticalForce;

	Color color;
public:
	Brick();
	Brick(float posX, float posY, float width, float length, float mass);

	float getPosX();
	float getPosY();
	float getVelX();
	float getVelY();
	float getAccX();
	float getAccY();
	float getWidth();
	float getHeight();
	float getHorizontalForce();
	float getVerticalForce();

	void setPosX(float posX);
	void setPosY(float posY);
	void setVelX(float velX);
	void setVelY(float velY);
	void setAccX(float accX);
	void setAccY(float accY);
	void setHorizontalForce(float horizontalForce);
	void setVerticalForce(float verticalForce);

	void Draw();
	void PrintStats();
	void CalculateAcceleration();
	void CalculateVelcoity();
	void CalculatePosition();
	void CheckBounds();
};

Brick::Brick()
	: posX(INITIAL_POS_X), posY(INITIAL_POS_Y), velX(0), velY(0), accX(0), accY(0), width(WIDTH), height(HEIGHT), mass(MASS), horizontalForce(0), verticalForce(0), color(RED) {}
Brick::Brick(float posX, float posY, float width, float height, float mass)
	: posX(posX), posY(posY), velX(0), velY(0), accX(0), accY(0), width(width), height(height), mass(mass), horizontalForce(0), verticalForce(0), color(RED) {}

float Brick::getPosX() { return posX; }
float Brick::getPosY() { return posY; }
float Brick::getVelX() { return velX; }
float Brick::getVelY() { return velY; }
float Brick::getAccX() { return accX; }
float Brick::getAccY() { return accY; }
float Brick::getWidth() { return width; }
float Brick::getHeight() { return height; }
float Brick::getHorizontalForce() { return horizontalForce; }
float Brick::getVerticalForce() { return verticalForce; }

void Brick::setPosX(float posX) { this->posX = posX; }
void Brick::setPosY(float posY) { this->posY = posY; }
void Brick::setVelX(float velX) { this->velX = velX; }
void Brick::setVelY(float velY) { this->velY = velY; }
void Brick::setAccX(float accX) { this->accX = accX; }
void Brick::setAccY(float accY) { this->accY = accY; }
void Brick::setHorizontalForce(float horizontalForce) { this->horizontalForce = horizontalForce; }
void Brick::setVerticalForce(float verticalForce) { this->verticalForce = verticalForce; }

void Brick::Draw() { DrawRectangle(posX, posY, width, height, color); }
void Brick::PrintStats()
{
	char message[50];
	int fontSize = 30;
	int textPosX = GetScreenWidth() / 3 * 2;
	int textPosY = fontSize / 2;

	strcpy_s(message, "Position: (");
	strcat_s(message, to_string(static_cast<int>(posX)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, to_string(static_cast<int>(GetScreenHeight() - posY)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);

	textPosY += fontSize + fontSize / 2;
	strcpy_s(message, "Velocity: (");
	strcat_s(message, to_string(static_cast<int>(velX)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, to_string(static_cast<int>(velY)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);

	textPosY += fontSize + fontSize / 2;
	strcpy_s(message, "Acceleration: (");
	strcat_s(message, to_string(static_cast<int>(accX)).c_str());
	strcat_s(message, ", ");
	strcat_s(message, to_string(static_cast<int>(-1 * accY)).c_str());
	strcat_s(message, ")");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);

	textPosY += fontSize + fontSize / 2;
	strcpy_s(message, "Horizontal Force: ");
	strcat_s(message, to_string(static_cast<int>(horizontalForce)).c_str());
	strcat_s(message, " N");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);

	textPosY += fontSize + fontSize / 2;
	strcpy_s(message, "Vertical Force: ");
	strcat_s(message, to_string(static_cast<int>(verticalForce)).c_str());
	strcat_s(message, " N");
	DrawText(message, textPosX, textPosY, fontSize, YELLOW);
}
void Brick::CalculateAcceleration()
{
	accX = horizontalForce / mass * 1000;
	accY = verticalForce / mass* 1000;
}
void Brick::CalculateVelcoity()
{
	velX += accX * GetFrameTime();
	velY += accY * GetFrameTime();
}
void Brick::CalculatePosition()
{
	posX += velX * GetFrameTime();
	posY += velY * GetFrameTime();
}
void Brick::CheckBounds()
{
	if (posX < 0)
	{
		posX = 0;
		velX *= -1 * collisionLost;
	}
	else if (posX > GetScreenWidth() - width)
	{
		posX = GetScreenWidth() - width;
		velX *= -1 * collisionLost;
	}
	if (posY < 0)
	{
		posY = 0;
		velY *= -1 * collisionLost;
	}
	else if (posY > GetScreenHeight() / 2)
	{
		posY = GetScreenHeight() / 2;
		velY *= -1 * collisionLost;
	}
}

class Force {
private:
	float posX;
	float posY;
	float endAngle;
	float value;
	Color color;
public:
	Force();
	Force(float posX, float posY, float degree, float value, Color color);
	void setPosition(float posX, float posY);
	void setEndAngle(float endAngle);
	float getValue();
	void setValue(float value);
	void setColor(Color color);
	void incrementEndAngle();
	void decrementEndAngle();
	void incrementValue();
	void decrementValue();
	float getHorizontalForce();
	float getVerticalForce();
	void Draw();
};
 
Force::Force() : posX(0), posY(0), endAngle(0), value(0), color(GREEN) {}
Force::Force(float posX, float posY, float degree, float value, Color color) 
	: posX(posX), posY(posY), endAngle(degree), value(value), color(color) {}
void Force::setPosition(float posX, float posY)
{
	this->posX = posX;
	this->posY = posY;
}
void Force::setEndAngle(float endAngle) { this->endAngle = endAngle; }
float Force::getValue() { return value; }
void Force::setValue(float value) { this->value = value; }
void Force::setColor(Color color) { this->color = color; }
void Force::incrementEndAngle() 
{
	endAngle++; 
	if (endAngle > 359)
		endAngle = 0;
}
void Force::decrementEndAngle() 
{
	endAngle--;
	if (endAngle < 0)
		endAngle = 359;
}
void Force::incrementValue() 
{ 
	value++; 
	if (value > forceMaxCap)
		value = forceMaxCap;
}
void Force::decrementValue() 
{ 
	value--;
	if (value < forceMinCap)
		value = forceMinCap;
}

float Force::getHorizontalForce() { return static_cast<float>(sin(endAngle * PI / 180) * value); }
float Force::getVerticalForce() { return static_cast<float>(cos(endAngle * PI / 180) * value); }
void Force::Draw()
{
	DrawCircleSector({ posX, posY }, 30, 0, endAngle, 100, color);
	int lineLength = 200;
	float endX = posX + lineLength * static_cast<float>(sin(endAngle * PI / 180));
	float endY = posY + lineLength * static_cast<float>(cos(endAngle * PI / 180));
	DrawLineEx({ posX, posY }, {endX, endY }, 3, WHITE);
	DrawCircle(endX, endY, 10, color);

	char message[50];
	int fontSize = 30;
	int border = 40;

	strcpy_s(message, "");
	strcat_s(message, to_string(static_cast<int>(endAngle)).c_str());
	strcat_s(message, " Degrees");
	DrawText(message, posX + border, posY - fontSize / 2, fontSize, color);

	strcpy_s(message, "");
	strcat_s(message, to_string(static_cast<int>(value)).c_str());
	strcat_s(message, " N");
	DrawText(message, posX + border, posY + fontSize / 2, fontSize, color);
}

//Global Variables
//-------------------------------------------------------
Brick brick;
float kineticFrictionRatio;
float staticFrictionRatio;

int toggledNum;
toggledElement toggled;
vector<Force> Forces;
//-------------------------------------------------------

void DrawFloor()
{
	int thickness = 3;
	DrawLineEx({ 0, GetScreenHeight() / 2 + HEIGHT + thickness }, { static_cast<float>(GetScreenWidth()), GetScreenHeight() / 2 + HEIGHT + thickness}, 3, WHITE);
}

void PrintToggledElement()
{
	char message[50];
	int fontSize = 30;
	int border = 40;
	int textPosY = 0;

	strcpy_s(message, "Toggled Element: ");
	if (toggled == leftForce) strcat_s(message, " Left Force");
	else if (toggled == rightForce) strcat_s(message, " Right Force");
	else if (toggled == kineticFriction) strcat_s(message, " Kinetic Friction Ratio");
	else if (toggled == staticFriction) strcat_s(message, " Static Friction Ratio");
	DrawText(message, border, textPosY, fontSize, YELLOW);

	textPosY += fontSize + 10;
	if (kineticFrictionRatio < 1)
	{
		strcpy_s(message, "Kinetic Friction Ratio: 0.");
		strcat_s(message, to_string(static_cast<int>(kineticFrictionRatio * 10)).c_str());
	}
	else
	{
		strcpy_s(message, "Kinetic Friction Ratio: ");
		strcat_s(message, to_string(static_cast<int>(kineticFrictionRatio)).c_str());
	}
	if (toggledNum == 2) DrawText(message, border, textPosY, fontSize, BLUE);
	else DrawText(message, border, textPosY, fontSize, YELLOW);

	textPosY += fontSize + 10;
	if (staticFrictionRatio < 1)
	{
		strcpy_s(message, "Static Friction Ratio: 0.");
		strcat_s(message, to_string(static_cast<int>(staticFrictionRatio * 10)).c_str());
	}
	else
	{
		strcpy_s(message, "Static Friction Ratio: ");
		strcat_s(message, to_string(static_cast<int>(staticFrictionRatio)).c_str());
	}
	if (toggledNum == 3) DrawText(message, border, textPosY, fontSize, BLUE);
	else DrawText(message, border, textPosY, fontSize, YELLOW);
}

void ForcesSetPosition()
{
	Forces.at(0).setPosition(brick.getPosX(), brick.getPosY() + brick.getHeight() / 2); //Left Force
	Forces.at(1).setPosition(brick.getPosX() + brick.getWidth(), brick.getPosY() + brick.getHeight() / 2); //Right Force
	Forces.at(2).setPosition(brick.getPosX(), brick.getPosY() + brick.getHeight()); //Force Of Friction
	Forces.at(3).setPosition(brick.getPosX() + brick.getWidth() / 2, brick.getPosY() + brick.getHeight()); //Force Due To Gravity
	Forces.at(4).setPosition(brick.getPosX() + brick.getWidth() / 2, brick.getPosY()); //Normal Force
}

void SetUp()
{
	brick = Brick(GetScreenWidth() / 2, GetScreenHeight() / 2, WIDTH, HEIGHT, MASS);
	kineticFrictionRatio = 0.5;
	staticFrictionRatio = 0.8;

	toggledNum = 0;
	toggled = leftForce;
	
	Forces.push_back(Force(brick.getPosX(), brick.getPosY() + brick.getHeight() / 2, 270, 0, BLUE)); //Left Force
	Forces.push_back(Force(brick.getPosX() + brick.getWidth(), brick.getPosY() + brick.getHeight() / 2, 90, 0, GREEN)); //Right Force
	Forces.push_back(Force(brick.getPosX(), brick.getPosY() + brick.getHeight(), 0, 0, GREEN)); //Force Of Friction
	Forces.push_back(Force(brick.getPosX() + brick.getWidth() / 2, brick.getPosY() + brick.getHeight(), 0, GRAVITY, GREEN)); //Force Due To Gravity
	Forces.push_back(Force(brick.getPosX() + brick.getWidth() / 2, brick.getPosY(), 180, GRAVITY, GREEN)); //Normal Force

}

void Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);
	brick.Draw();
	brick.PrintStats();
	PrintToggledElement();
	DrawFloor();
	for (int i = 0; i < Forces.size(); i++)
		Forces.at(i).Draw();
	EndDrawing();
}

void Input()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		if (toggledNum < 2) Forces.at(toggledNum).setColor(GREEN);
		else Forces.at(2).setColor(GREEN);
		toggledNum++;
		if (toggledNum == 4) toggledNum = 0;
		if (toggledNum < 2) Forces.at(toggledNum).setColor(BLUE);
		else Forces.at(2).setColor(BLUE);
		toggled = toggledElement(toggledNum);
	}
	if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) //Increase angle
	{
		if (toggledNum < 2)
			Forces.at(toggledNum).incrementEndAngle();
	}
	if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) //Decrease angle
	{
		if (toggledNum < 2)
			Forces.at(toggledNum).decrementEndAngle();
	}
	if (IsKeyPressed(KEY_D)) //Increase initial velocity
	{
		if (toggledNum < 2)
			Forces.at(toggledNum).incrementValue();
		else if (toggledNum == 2)
		{
			kineticFrictionRatio += 0.1;
			if (kineticFrictionRatio > 1) kineticFrictionRatio = 1;
			if (kineticFrictionRatio > staticFrictionRatio) staticFrictionRatio = kineticFrictionRatio;
		}
		else if (toggledNum == 3)
		{
			staticFrictionRatio += 0.1;
			if (staticFrictionRatio > 1) staticFrictionRatio = 1;
		}
	}
	if (IsKeyPressed(KEY_A)) //Decrease initial velocity
	{
		if (toggledNum < 2)
			Forces.at(toggledNum).decrementValue();
		else if (toggledNum == 2)
		{
			kineticFrictionRatio -= 0.1;
			if (kineticFrictionRatio < 0) kineticFrictionRatio = 0;
		}
		else if (toggledNum == 3)
		{
			staticFrictionRatio -= 0.1;
			if (staticFrictionRatio < 0) staticFrictionRatio = 0;
			if (kineticFrictionRatio > staticFrictionRatio) kineticFrictionRatio = kineticFrictionRatio;
		}
	}
	if (IsKeyPressed(KEY_R) || IsKeyDown(KEY_R)) //Reset
	{
		Forces.clear();
		SetUp();
	}
}

void Logic()
{
	//Calculate Normal Force
	Forces.at(4).setValue(GRAVITY + Forces.at(0).getVerticalForce() + Forces.at(1).getVerticalForce());
	if (Forces.at(4).getValue() < 0 || (brick.getPosY() < GetScreenHeight() / 2 - 2))
		Forces.at(4).setValue(0);

	//Calculate Force of Friction
	/*if ((brick.getPosY() > GetScreenHeight() / 2 - 2))
	{
		Forces.at(2).setValue((Forces.at(0).getHorizontalForce() + Forces.at(1).getHorizontalForce()));
		if (Forces.at(2).getValue() > 0) Forces.at(2).setEndAngle(270);
		else if (Forces.at(2).getValue() < 0)
		{
			Forces.at(2).setEndAngle(90);
			Forces.at(2).setValue((Forces.at(2).getValue() * -1));
		}
		if (Forces.at(2).getValue() > Forces.at(4).getValue() * staticFrictionRatio || brick.getHorizontalForce() != 0)
		{
			if (Forces.at(2).getValue() >= Forces.at(4).getValue() * kineticFrictionRatio)
				Forces.at(2).setValue(Forces.at(4).getValue() * kineticFrictionRatio);
		}
	}
	else Forces.at(2).setValue(0);*/

	//Calculate Force of Friction Fixed
	if ((brick.getPosY() > GetScreenHeight() / 2 - 2))
	{
		float netHorizontalForceLeftAndRight = Forces.at(0).getHorizontalForce() + Forces.at(1).getHorizontalForce();
		float staticFriction = Forces.at(4).getValue() * staticFrictionRatio;
		float kineticFriction = Forces.at(4).getValue() * kineticFrictionRatio;

		if (netHorizontalForceLeftAndRight > 0) Forces.at(2).setEndAngle(270);
		else Forces.at(2).setEndAngle(90);
		if (brick.getVelX() < 1 && brick.getVelX() > -1 ) //There is no motion
		{
			if (abs(netHorizontalForceLeftAndRight) <= staticFriction)
			{
				Forces.at(2).setValue(abs(netHorizontalForceLeftAndRight));
			}
			else
			{
				if (abs(netHorizontalForceLeftAndRight) <= kineticFriction) Forces.at(2).setValue(abs(netHorizontalForceLeftAndRight));
				else Forces.at(2).setValue(kineticFriction);
			}
		}
		else
		{
			if (abs(netHorizontalForceLeftAndRight) <= kineticFriction) Forces.at(2).setValue(abs(netHorizontalForceLeftAndRight));
			else Forces.at(2).setValue(kineticFriction);
		}
	}
	else Forces.at(2).setValue(0);

	//Calculating net horizontal and vertical forces of brick
	brick.setHorizontalForce(0);
	brick.setVerticalForce(0);
	for (int i = 0; i < Forces.size(); i++)
	{
		brick.setHorizontalForce(brick.getHorizontalForce() + Forces.at(i).getHorizontalForce());
		brick.setVerticalForce(brick.getVerticalForce() + Forces.at(i).getVerticalForce());
	}
	brick.CalculateAcceleration();
	brick.CalculateVelcoity();
	brick.CalculatePosition();
	brick.CheckBounds();
	ForcesSetPosition();
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics Pushing and Pulling Weight");
	SetWindowState(FLAG_VSYNC_HINT);
	SetUp();
	while (!WindowShouldClose())
	{
		Draw();
		Input();
		Logic();
	}
	CloseWindow();
	return 0;
}