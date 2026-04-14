#pragma once

#include <srl.hpp>
#include <vector>
using namespace SRL::Types;
using namespace SRL::Math::Types;




class catmullrom 
{
        
        

        Fxp m[4][4] = {	{ -0.5f,  1.5f, -1.5f,  0.5f},
			{ 1.0f, -2.5f,  2.0f, -0.5f},
			{-0.5f,  0.0f,  0.5f,  0.0f},
			{ 0.0f,  1.0f,  0.0f,  0.0f}};

        public : 

        std::vector<Vector3D> control_points; // a control point is a Vector3D in space
        std::vector<Vector2D> curve_points2D;
        std::vector<Vector3D> curve_points3D;

        uint16_t control_points_size = 0;


        size_t addControlPoint(Vector3D a)
        {
                this->control_points.push_back(a);
                this->control_points_size = this->control_points.size();
                return control_points_size;
        }

        void getGlobalCalmullRomPoint(Fxp gt, Vector3D *res)
        {
                // 
                Fxp t = gt * this->control_points_size;
                int16_t index = t.Floor().As<int16_t>();        //which segment

                t = t - index;  // where withing the segment

                int indices[4] = {0};
                indices[0] = (index + this->control_points_size-1)%this->control_points_size;
                indices[1] = (indices[0] + 1) % this->control_points_size;
                indices[2] = (indices[1] + 1) % this->control_points_size;
                indices[3] = (indices[2] + 1) % this->control_points_size;

                getCalmullRomPoint(t, indices, res);
        }

        void getCalmullRomPoint(Fxp t, int * indices, Vector3D * res)
        {
                Fxp res_aux[4] = {0.0};

                for(int i = 0 ; i < 4 ; i++)
                {
                        res_aux[i] = t.Pow(3) * this->m[0][i] + t.Pow(2) * this->m[1][i] + t * this->m[2][i] + this->m[3][i];
                }
                
                res->X = (res_aux[0] * this->control_points[indices[0]].X) + (res_aux[1] * this->control_points[indices[1]].X) + (res_aux[2] * this->control_points[indices[2]].X);
                res->Y = (res_aux[0] * this->control_points[indices[0]].Y) + (res_aux[1] * this->control_points[indices[1]].Y) + (res_aux[2] * this->control_points[indices[2]].Y);
                res->Z = (res_aux[0] * this->control_points[indices[0]].Z) + (res_aux[1] * this->control_points[indices[1]].Z) + (res_aux[2] * this->control_points[indices[2]].Z);
        }

        void getGlobalCalmullRomPointTangent(Fxp gt, Vector3D *res)
        {
                // 
                Fxp t = gt * this->control_points_size;
                int16_t index = t.Floor().As<int16_t>();        //which segment

                t = t - index;  // where withing the segment

                int indices[4] = {0};
                indices[0] = (index + this->control_points_size-1)%this->control_points_size;
                indices[1] = (indices[0] + 1) % this->control_points_size;
                indices[2] = (indices[1] + 1) % this->control_points_size;
                indices[3] = (indices[2] + 1) % this->control_points_size;

                getCalmullRomPointTangent(t, indices, res);
        }

         void getCalmullRomPointTangent(Fxp t, int * indices, Vector3D * res)
        {
                Fxp res_aux[4] = {0.0};

                for(int i = 0 ; i < 4 ; i++)
                {
                        res_aux[i] = (2 * t.Pow(2)) * this->m[0][i] + (2 * t) * this->m[1][i] + this->m[2][i];
                }
                
                res->X = (res_aux[0] * this->control_points[indices[0]].X) + (res_aux[1] * this->control_points[indices[1]].X) + (res_aux[2] * this->control_points[indices[2]].X);
                res->Y = (res_aux[0] * this->control_points[indices[0]].Y) + (res_aux[1] * this->control_points[indices[1]].Y) + (res_aux[2] * this->control_points[indices[2]].Y);
                res->Z = (res_aux[0] * this->control_points[indices[0]].Z) + (res_aux[1] * this->control_points[indices[1]].Z) + (res_aux[2] * this->control_points[indices[2]].Z);
        }

        void Draw2D(Fxp step, Fxp from = 0.0, Fxp to = 1.0)
        {
                Fxp gtt = from;
                Vector3D res = Vector3D(0.0);
                
                for( ; gtt < to ; gtt += step)
                {
                        getGlobalCalmullRomPoint(gtt, &res);
                        Vector2D r = Vector2D(res.X, res.Y);
                        this->curve_points2D.push_back(r);
                }

                // Desenhar a curva
                size_t size = this->curve_points2D.size();
                SRL::Debug::Print(1,2, "Nr Points %d", size);

                for(int i = 0 ; i < size-1 ; i++)
                {
                        SRL::Scene2D::DrawLine(this->curve_points2D[i], this->curve_points2D[i+1], HighColor::Colors::White, 500);
                }

                this->curve_points2D.clear();
        }
        
        

};

