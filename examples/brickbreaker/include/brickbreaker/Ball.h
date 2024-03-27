#pragma once

#include "brickbreaker/Brick.h"
#include "brickbreaker/Paddle.h"
#include "brickbreaker/ScreenBorder.h"

#include "spark/core/Application.h"
#include "spark/core/GameObject.h"
#include "spark/core/components/Circle.h"
#include "spark/core/components/Collider.h"
#include "spark/core/components/Transform.h"
#include "spark/lib/Random.h"
#include "spark/patterns/Signal.h"
#include "spark/rtti/HasRtti.h"

#include <numbers>

namespace brickbreaker
{
    /**
     * \brief A ball that bounces around the screen.
     */
    class Ball final : public spark::core::GameObject
    {
        DECLARE_SPARK_RTTI(Ball, GameObject)

    public:
        spark::math::Vector2<float> direction = {};
        float velocity = 400.0f;
        spark::patterns::Signal<> onLoose;
        bool goingUp = false;
        bool goingDown = false;
        bool goingLeft = false;
        bool goingRight = false;
        bool gameStarted = false;


    public:
        explicit Ball(std::string name, GameObject* parent, const float radius)
            : GameObject(std::move(name), parent)
        {
            addComponent<spark::core::components::Circle>(radius);
            addComponent<spark::core::components::DynamicCollider>(spark::math::Rectangle<float> {{0, 0}, {radius * 2, radius * 2}});

            component<spark::core::components::DynamicCollider>()->onCollision.connect([this](const spark::core::components::Collider& other)
            {
                if (&other.gameObject()->rttiInstance() == &Brick::classRtti())
                {
                    const auto bounds = component<spark::core::components::DynamicCollider>()->bounds();
                    const auto other_bounds = other.bounds();

                    /*  
                    * check if the ball hit the brick from the top, bottom, left or right
                    * and check if the center of the ball is near the center of the brick
                    */                     

                    const bool top = bounds.w - other_bounds.y > 0;
                    const bool bottom = bounds.y - other_bounds.w  > 0;
                    const bool left = bounds.x - other_bounds.z > 0;
                    const bool right = bounds.z - other_bounds.x > 0;

                    if (top && !goingUp)
                    {
                        direction = { direction.x, -direction.y };
                        
                        goingUp = true;
                    }
                    else if (bottom && goingUp) 
                    {
                        direction = { direction.x, -direction.y };
                        goingUp = false;
         
                    }
                    else if (left && !goingLeft)
                    {
                        direction = { -direction.x, direction.y };
                        goingLeft = true;
                    }
                    else if (right && goingLeft) 
                    {
                        direction = { -direction.x, direction.y };
                        goingLeft = false;
                        
                    }
                    

                    // Increase the velocity by 0.1%.
                    velocity *= 1.001f;

                    // Destroy the brick.
                    Destroy(const_cast<GameObject*>(other.gameObject()));
                }
                else if (&other.gameObject()->rttiInstance() == &Paddle::classRtti())
                {
                    /* we get a % of the paddle to know where the ball hit and change the direction accordingly to that %
                     * but from 45% to 55% angle is 0, from 0% to 45% angle is change proportionally to the % of the paddle on the left side
                     * and from 55% to 100% angle is change proportionally to the % of the paddle on the right side
                    */
                    float paddle_Size = m_paddle->component<spark::core::components::Rectangle>()->size.x;
                    float paddle_Position = m_paddle->transform()->position.x;
                    float ball_Position = transform()->position.x + component < spark::core::components::Circle>()->radius;
                    float paddle_Percentage = (ball_Position - paddle_Position) / paddle_Size;

                    if (paddle_Percentage < 0.40f) // 0% to 40%
                    {
                        //the more the ball is on the left side the more the angle is negative
                        float angle = std::numbers::pi_v<float> * paddle_Percentage;
                        direction = {-std::cos(angle), -std::sin(angle)};

                        
                        goingLeft = true;
                    }
                    else if (paddle_Percentage > 0.60f) // 60% to 100%
                    {
                        //the more the ball is on the right side the more the angle is positive
                        float angle = std::numbers::pi_v<float> * (1 - paddle_Percentage);
                        direction = { std::cos(angle), -std::sin(angle) };

                        goingLeft = false;
                        
                    }
                    else if(paddle_Percentage > 0.40f && paddle_Percentage < 0.60f) // 40% to 60%
                    {
                        direction = {0, -direction.y};

                        goingLeft = false;
                        
                    }
                    
                    // Increase the velocity by 0.1%.
                    velocity *= 1.001f;

                } else if(&other.gameObject()->rttiInstance() == &ScreenBorder::classRtti())
                {
                    if (other.gameObject()->name() == "Top Border") 
                    {
                        direction = { direction.x, -direction.y };
                        
                        goingUp = false;
                    }                        
                    else if (other.gameObject()->name() == "Left Border") 
                    {
                        direction = { -direction.x, direction.y };
                        
                        goingLeft = false;
                    } 
                    else if (other.gameObject()->name() == "Right Border") 
                    {
                        direction = { -direction.x, direction.y };
                        goingLeft = true;
                        
                    }  
                }
            });
        }

        void onSpawn() override
        {
            // Set the ball in the middle of the screen.
            const auto window_size = spark::core::Application::Instance()->window().size().castTo<float>();
            transform()->position = { window_size.x / 2, window_size.y * 0.85f };

            m_paddle = FindByName(root(), "Paddle");

            

            
        }

        void onUpdate(const float dt) override
        {
            
            if (checkLoose(transform()->position))
                onLoose.emit();

            if(transform()->position.y > m_paddle->transform()->position.y + 15.0f)
            {
                m_health--;
                if(m_health == 0)
                    onLoose.emit();
                else
                    onSpawn();
            }

            if(!gameStarted)
            {
                const float next_position = spark::core::Input::MousePosition().x;
                transform()->position.x = next_position;
            }

            if (spark::core::Input::IsMousePressed() && !gameStarted)
            {
                direction = { 0, -1 };
                transform()->position += direction * velocity * dt;
                gameStarted = true;
                goingUp = true;
            }
            else
            {
                transform()->position += direction * velocity * dt;
            }
                
                
        }

    private:
        /**
         * \brief Checks if the ball is going to be outside the screen boundaries.
         * \param next_position The desired position of the ball calculated from the current position and direction for the next frame.
         * \return A boolean indicating if the ball is going to be outside the screen boundaries (if the player has lost).
         */
        [[nodiscard]] static bool checkLoose(const spark::math::Vector2<float>& next_position)
        {
            const auto window_height = spark::core::Application::Instance()->window().size().castTo<float>().y;
            if (next_position.y > window_height)
                return true;
            return false;
        }

    private :
        GameObject* m_paddle = nullptr;

        //health of the ball
        int m_health = 3;
    };
}

IMPLEMENT_SPARK_RTTI(brickbreaker::Ball)
