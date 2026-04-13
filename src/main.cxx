#include <srl.hpp>
#include "curves.hpp"

// Using to shorten names for Vector and HighColor
using namespace SRL::Types;
using namespace SRL::Math::Types;

// Main program entry
int main()
{
	SRL::Core::Initialize(HighColor(20,10,50));
    SRL::Debug::Print(1,1, "Catmull sample - 2D Curve");
    

    // Initialize random number generator with a specific seed
    SRL::Math::Random rnd = SRL::Math::Random(15);

    // Define variables to keep track of random data
    int32_t data[320];
    uint32_t pointer = 0;

    // Update delay counter
    uint32_t tm = 0;
    
    catmullrom curve;

    // precisamos de 4 pontos de controlo. para testar , fazer uma curva em 2d

    Vector3D control_points[6] = {Vector3D(0.0)};

    control_points[0] = Vector3D(-100, 10, 0);
    control_points[1] = Vector3D(-80, -10, 0);
    control_points[2] = Vector3D(-30, 10, 0);
    control_points[3] = Vector3D(30, -10, 0);
    control_points[4] = Vector3D(80, 50, 0);
    control_points[5] = Vector3D(100, -10, 0);

    for(int i = 0 ; i < 6 ; i++)
    {
        curve.addControlPoint(control_points[i]);
    }
    
    // Main program loop
	while(1)
	{
       
        
            Vector2D a = Vector2D(control_points[0].X,  control_points[0].Y);
            Vector2D b = Vector2D(control_points[1].X,  control_points[1].Y);
            Vector2D c = Vector2D(control_points[2].X,  control_points[2].Y);
            Vector2D d = Vector2D(control_points[3].X,  control_points[3].Y);
            Vector2D e = Vector2D(control_points[4].X,  control_points[4].Y);
            Vector2D f = Vector2D(control_points[5].X,  control_points[5].Y);
            
            SRL::Scene2D::DrawLine(a, b , HighColor::Colors::White,  500.0);
            SRL::Scene2D::DrawLine(b, c , HighColor::Colors::Blue,  500.0);
            SRL::Scene2D::DrawLine(c, d , HighColor::Colors::Red,  500.0);
            SRL::Scene2D::DrawLine(d, e , HighColor::Colors::Green,  500.0);
            SRL::Scene2D::DrawLine(e, f , HighColor::Colors::Magenta,  500.0);

            curve.Draw2D(0.01);
                
        /*
        SRL::Scene2D::DrawLine(
            Vector2D(static_cast<int16_t>(pointer - 160), 50),
            Vector2D(static_cast<int16_t>(pointer - 160), -50),
            HighColor::Colors::Red,
            500.0);
        */
        SRL::Core::Synchronize();
	}

	return 0;
}
