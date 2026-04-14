#include <srl.hpp>
#include "curves.hpp"
#include "modelObject.hpp"

// Using to shorten names for Vector and HighColor
using namespace SRL::Types;
using namespace SRL::Math::Types;

// Load color palettes here
int16_t LoadPalette(SRL::Bitmap::BitmapInfo* bitmap)
{
    // Get free CRAM bank
    int32_t id = SRL::CRAM::GetFreeBank(bitmap->ColorMode);

    if (id >= 0)
    {
        SRL::CRAM::Palette palette(bitmap->ColorMode, id);

        if (palette.Load((HighColor*)bitmap->Palette->Colors, bitmap->Palette->Count) >= 0)
        {
            // Mark bank as in use
            SRL::CRAM::SetBankUsedState(id, bitmap->ColorMode, true);
            return id;
        }

        return id;
    }

    // No free bank found
    return -1;
}

int32_t loadTGA(char* filename) //texture loading function
{
        SRL::Bitmap::TGA *tga = new SRL::Bitmap::TGA(filename); // Loads TGA file into main RAM
        SRL::Bitmap::BitmapInfo info = tga->GetInfo();          // Get info about the tga we are loading
        int32_t textureIndex = -1;
        
        if(info.ColorMode == SRL::CRAM::TextureColorMode::RGB555) // RGBA texture
        {
            textureIndex = SRL::VDP1::TryLoadTexture(tga);  // Loads TGA into VDP1
        }
        else
        {
            //assume is pallet texture
            textureIndex = SRL::VDP1::TryLoadTexture(tga, LoadPalette);
        }
        
        delete tga; 
        
        if (textureIndex == -1)
        {
            SRL::Debug::AssertScreen("Failed loading texture %s", filename, "loadTGA", filename);
        } 
        
        return textureIndex;
}


// Main program entry
int main()
{
	SRL::Core::Initialize(HighColor(20,10,50));
    SRL::Debug::Print(1,1, "Catmull sample - 2D Curve");
    
    // load sprite

    int32_t textureIndex = loadTGA("TEST.TGA");


    ModelObject cube = ModelObject("CUBE01.NYA");
    Vector3D cameraLocation = Vector3D(12.5, -12.5, 12.5);
  
    // Setup light, we can use scale of the vector to manipulate light intensity
    Vector3D lightDirection = Vector3D(0.2, 0.0, 0.2);
    SRL::Scene3D::SetDirectionalLight(lightDirection);
    SRL::Scene3D::SetDepthDisplayLevel(4);

    catmullrom curve;

   
    Vector3D control_points[6] = {Vector3D(0.0)};

    control_points[0] = Vector3D(-100, 10, 0);
    control_points[1] = Vector3D(-80, -10, 0);
    control_points[2] = Vector3D(-30, 10, 0);
    control_points[3] = Vector3D(30, -10, 0);
    control_points[4] = Vector3D(80, 50, 0);
    control_points[5] = Vector3D(100, -10, 0);

    Vector3D points[4] = {Vector3D(0.0)};
    points[0] = Vector3D(-10, 0,-10);
    points[1] = Vector3D(5,  -10,-15);
    points[2] = Vector3D( 15, -20, 10);
    points[3] = Vector3D(-7, 10, 15);

    for(int i = 0 ; i < 6 ; i++)
    {
        curve.addControlPoint(control_points[i]);
    }
    
    catmullrom curve3d;

    for(int i = 0 ; i < 4 ; i++)
    {
        curve3d.addControlPoint(points[i]);
    }

    Fxp t = 0.0;

    // Main program loop
	while(1)
	{
            SRL::Scene3D::LoadIdentity();
            // Set camera location and direction
            curve3d.getGlobalCalmullRomPoint(t, &cameraLocation);
            t = t + 0.009;
            SRL::Scene3D::LookAt(cameraLocation, Vector3D(), Angle::FromDegrees(0.0));

            cube.Draw();
        


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
            Vector3D spriteLocation = Vector3D(0.0, 0.0, 500);
            Vector3D tangentLine = Vector3D(0.0, 0.0, 500);
            
            curve.getGlobalCalmullRomPoint(t, &spriteLocation);
            curve.getGlobalCalmullRomPointTangent(t, &tangentLine);
            spriteLocation.Z = 500.0;

            SRL::Scene2D::DrawLine(spriteLocation, tangentLine, HighColor::Colors::Yellow, 500);
            SRL::Scene2D::DrawSprite(textureIndex, spriteLocation);


           



                
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
