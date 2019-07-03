#include "../COMMON.H"


float defLight[3]={0, 0, -1.0f};
void calcLight(animated_model_t * aModel)
{
    for (uint32_t i=0; i<aModel->nbModels; i++ )
    {
       model_t * mod =  &aModel->model[i];
       for (uint32_t ii=0; ii<mod->nbPoint; ii++)
       {
            vertex_t * v = &mod->pntbl[ii];
            float d = -dotProduct((float*)v->normal, (float*)defLight);
            v->color[0]=d;
            v->color[1]=d;
            v->color[2]=d;

       }
    }
}
