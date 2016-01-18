#include "Player.h"	
#include <iostream>
Player::Player()
	: moveSpeed(250), maxSprint(500), sprintTime(maxSprint), maxHealth(5000), currentHealth(maxHealth), alive(true), clockStarted(false), canTakeDamage(true), invincTime(1.f)
{
}
bool Player::init()
{
	if (!initSpritesheet())
		return(false);
	
	collider_.width = getGlobalBounds().width * 0.90;
	collider_.height = getGlobalBounds().height * 0.90;

	sprintRect_.setFillColor(sf::Color::Red); //init sprint rect with colour red
	sprintRect_.setSize(sf::Vector2f(64, 5)); //init sprint rect with width of player and size of 5

	healthRect_.setFillColor(sf::Color::Green); //init sprint rect with colour red
	healthRect_.setSize(sf::Vector2f(64, 5)); //init sprint rect with width of player and size of 5

	return(true);
}

bool Player::initSpritesheet()
{
	if (!spritesheet_.loadFromFile("res//entities//spritesheet.png"))
		return(false);

	isAnimated(true);

	playerWalk_.setSpriteSheet(spritesheet_);

	const int SIZE(128);

	playerWalk_.addFrame(sf::IntRect(0 * SIZE, 2 * SIZE, SIZE, SIZE));
	playerWalk_.addFrame(sf::IntRect(1 * SIZE, 2 * SIZE, SIZE, SIZE));
	playerWalk_.addFrame(sf::IntRect(2 * SIZE, 2 * SIZE, SIZE, SIZE));
	playerWalk_.addFrame(sf::IntRect(3 * SIZE, 2 * SIZE, SIZE, SIZE));

	setAnimation(playerWalk_);
	setAnimationLoop(true);
	playAnimation();
	sf::Time frameTime = sf::milliseconds(150);
	setFrameTime(frameTime);

	return true;
}

void Player::update(const sf::Time& delta, const float rotation, const sf::RenderTexture* renderTexture)
{
	updateMovement(delta);
	updateSprintBar(renderTexture);
	updateHealthBar(renderTexture);
	updateRotation(rotation);
	updateAnimation(delta);
}

void Player::sprint()
{
	if (sprintTime > 0) //if the sprint timer is greater then 0 then allow sprinting
	{
		moveSpeed = 1000;
		--sprintTime; //decrease sprint timer towards 0
	}
	else
	{
		moveSpeed = 200; //sprint timer is equal or less than 0 change move speed to walking pace
	}

}

void Player::walk()
{

	moveSpeed = 200; //make sure move speed is walking pace
	if (sprintTime < maxSprint) //if sprint timer is less than the max sprint duration then
	{
		sprintTime += 0.5f; //increase sprint timer back up towards the max duration
	}
}

void Player::updateSprintBar(const sf::RenderTexture* renderTexture)
{
	float scaleX = sprintTime / maxSprint; //get percentage of sprint timer
	sprintRect_.setPosition(renderTexture->mapPixelToCoords(sf::Vector2i(10, 100)));
	sprintRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
}

void Player::updateHealthBar(const sf::RenderTexture* renderTexture)
{
	float scaleX = currentHealth / maxHealth; //get percentage of sprint timer
	healthRect_.setPosition(renderTexture->mapPixelToCoords(sf::Vector2i(10, 80)));
	healthRect_.setScale(scaleX, 1); //set the rects size based on sprint timer
}

void Player::updateMovement(const sf::Time& delta)
{
	sf::Vector2f movement(0, 0);
	sf::Vector2f direction(0, 0);
	collider_.top = getPosition().y - collider_.height / 2.f;
	collider_.left = getPosition().x - collider_.width / 2.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))	//press shift and sprint
	{
		sprint();
	}
	else //otherwise walk
	{
		walk();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{ //if a directional key is pressed
		playAnimation();
		getDirection(direction);

		//create a vector that uses the two colliders and the direction to work out collisions
		sf::Vector2f a(direction.x * (delta.asSeconds() * moveSpeed), direction.y * (delta.asSeconds() * moveSpeed));

		movement = (p_tileMap_->getCollisionVector(collider_, a, getID()));
		normalizeMovement(movement);
		move(movement);	//move the player
	}
	else
	{
		stopAnimation();
		setFrame(1);
	}

}

void Player::updateRotation(const float rotation)
{
	setRotation(rotation);
}

bool Player::invincibility()
{
	if (canTakeDamage == false)
	{
		if (clockStarted == false)
		{
			clockStarted = true;

			invincClock_.restart();
		}
		invincTimer_ = invincClock_.getElapsedTime();
		if (invincTimer_.asSeconds() >= invincTime)
		{
			canTakeDamage = true;
			clockStarted = false;
			return true;
		}
	}
	return false;
}

void Player::resetHealth()
{
	currentHealth = maxHealth;
}

void Player::getDirection(sf::Vector2f& direction)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{//if up key is pressed change direction y vector to -1 and move the y collider above the player
		direction.y = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{//if down key is pressed change direction y vector to 1 and move the y collider below the player
		direction.y = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{//if left key is pressed change direction x vector to -1 and move the x collider left of the player
		direction.x = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{//if left key is pressed change direction x vector to 1 and move the x collider right of the player
		direction.x = 1;
	}
}

void Player::normalizeMovement(sf::Vector2f& movement)
{
	if (movement.x != 0 && movement.y != 0) //if the movement vector is not (0,0)
	{
		sf::Vector2f normalized(normalize(movement));
		movement.x *= fabs(normalized.x);
		movement.y *= fabs(normalized.y);
	}
}

