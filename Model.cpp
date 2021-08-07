#include<vector>
#include<string>
class Form
{
	public:
		Form(float formType, std::vector<float> pos, std::vector<float> size, std::vector<float> color)
		{
			this->formType = formType;
			this->pos = pos;
			this->size = size;
			this->color = color;
		}
  
		Form(float formType, float x, float y, float z, float sx, float sy, float sz, float r, float g, float b)
		{
			this->formType = formType;
			this->pos = {x, y, z};
			this->size = {sx, sy, sz};
			this->color = {r, g, b};
		}
		std::vector<float> getPos()
		{
			return this->pos;
		}
		std::vector<float> getSIze()
		{
			return this->size;
		}
		std::vector<float> getColor()
		{
			return this->color;
		}
		float getFormType()
		{
			return this->formType;
		}

		void setPos(std::vector<float> pos)
		{
			for (int value; value < 3; value++)
			{
				this->pos[value] = pos[value];
			}
		}
		void setPos(int index, float value)
		{
			this->pos[index] = value;
		}

		void setSize(std::vector<float> size)
		{
			for (int value; value < 3; value++)
			{
				this->size[value] = size[value];
			}
		}
		void setSize(int index, float value)
		{
			this->size[index] = value;
		}

		void setColor(std::vector<float> colors)
		{
			for (int value; value < 3; value++)
			{
				this->color[value] = color[value];
			}
		}
		void setColor(int index, float value)
		{
			this->color[index] = value;
		}	

		void SetFormType(float formType)
		{
			this->formType = formType;
		}
	private:
		float formType;
		std::vector<float> pos;
		std::vector<float> size;
		std::vector<float> color;
};
class Model
{
	public:
		Model(std::string name, float pos[3], std::vector<Form> forms, std::vector<Model> models)
		{
			this->name = name;
			this->forms = forms;
			this->models = models;
			for (int value; value < 3; value++)
			{
				this->pos[value] = pos[value];
			}
		}
		Model(std::string name, float x, float y, float z, std::vector<Form> forms, std::vector<Model> models)
		{
			this->name = name;
			this->forms = forms;
			this->models = models;
			this->pos = {x, y, z};
		}

		void setPos(std::vector<float> pos)
		{
			for (int value; value < 3; value++)
			{
				this->pos[value] = pos[value];
			}

		}
		void setPos(int index, float value)
		{
			this->pos[index] = value;
		}
		std::vector<Form> forms;
		std::vector<Model> models;
	private:
		std::vector<float> pos;
		std::string name;

};
int main()
{
	return 0;
}