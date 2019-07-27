#include <iostream>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;


class Texto{

private:
	Font fuente;
	Text texto;
	RenderWindow* ventana;
	int tamaño;
	string mensaje;
	string fuentes;
	float pos_x, pos_y;
	Color color;

public:
	Texto(RenderWindow* ventana,string fuentes,string mensaje,Color color,int tamaño)
	{
		this->ventana = ventana;
		this->mensaje = mensaje;
		this->tamaño = tamaño;
		this->fuente = fuente;
		this->color = color;

		fuente.loadFromFile(fuentes);
		texto.setFont(fuente);
		texto.setCharacterSize(tamaño);
		texto.setString(mensaje);
		texto.setFillColor(color);
	}

	float getSize_x()
	{
		return this->texto.getGlobalBounds().width;
	}

	float getSize_y()
	{
		return this->texto.getGlobalBounds().height;

	}

	void setPos(float x, float y)
	{
		texto.setPosition(x, y);
	}

	void render()
	{
		this->ventana->draw(this->texto);
	}
};

class Puntero {

	private:
		float x, y;
		Texture Tpuntero;
		Sprite Spuntero;
		string textura;
		RenderWindow* ventana;
		int opcion;

	public:
		Puntero(int x, int y,RenderWindow* ventana)
		{
			this->textura = textura;
			this->ventana = ventana;
			Tpuntero.loadFromFile(textura);
			Spuntero.setPosition(x, y);
			Spuntero.setScale(Vector2f(0.1, 0.1));
		}

		void update()
		{

		}

		void render()
		{
			this->ventana->draw(this->Spuntero);
		}
};

class Jugador {
	
private:
	
	float x, y;
	float velocidad=3.5;

public:
	
	Texture Tperro;
	Sprite Sperro;
	RenderWindow* ventana;

	Jugador(float x,float y,RenderWindow *ventana)
	{
		this->ventana = ventana;
		this->x = x;
		this->y = y;

		if (!Tperro.loadFromFile("Texturas/doge.png"))
			cout << "No se pudo cargar la textura" << endl;

		Sperro.setTexture(Tperro);
		Sperro.setPosition(x, y);	
		Sperro.setScale(Vector2f(0.05f, 0.05f));
	}

	void update()
	{
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			x-=velocidad;
			Sperro.setPosition(x,y);
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			x+=velocidad;
			Sperro.setPosition(x, y);
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			y-=velocidad;
			Sperro.setPosition(x, y);
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			y+=velocidad;
			Sperro.setPosition(x, y);
		}
	}

	void render()
	{
		this->ventana->draw(this->Sperro);
	}
};

class Nivel
{
public:
	Texture Tfondo;
	Sprite Sfondo;
	RenderWindow* ventana;
	Event evento;
	bool nivel_activo = true;

	Jugador *j;

	Nivel(string ruta_fondo, Vector2u ventana_escala,RenderWindow *ventana,Event evento)
	{
		this->ventana = ventana;
		this->evento = evento;

		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana_escala.x/Sfondo.getGlobalBounds().width, ventana_escala.y / Sfondo.getGlobalBounds().height));
		
		j = new Jugador(40, 80, ventana);
	}

	void loop()
	{
		while (nivel_activo)
		{
			eventos();
			
			update();

			render();
		}
	}

	void update()
	{
		this->j->update();
	}

	void eventos()
	{
		while (this->ventana->pollEvent(this->evento))
		{
			if (this->evento.type == Event::Closed)
			{
				this->ventana->close();
				exit(1);
			}
		}
	}

	void render()
	{
		this->ventana->clear();
		
		this->ventana->draw(this->Sfondo);
		this->j->render();

		this->ventana->display();
	}
};

class Menu
{
	private:

		RenderWindow* ventana;
		Event evento;
		Texture Tmenu;
		Sprite Smenu;
		bool menu_activo = true;
		Texto *cats_invasion,*jugar;
		Puntero p1;

	public:
		Menu(RenderWindow *ventana, Vector2u ventana_escala, Event evento)
		{
			this->ventana = ventana;
			Tmenu.loadFromFile("Texturas/menu.jpg");
			Smenu.setTexture(Tmenu);
			Smenu.setScale(Vector2f(ventana_escala.x / Smenu.getGlobalBounds().width, ventana_escala.y / Smenu.getGlobalBounds().height));

			cats_invasion = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "Cat's Invasion", Color::Blue, 50);
			cats_invasion->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f, this->ventana->getSize().y / 6.f);

			jugar = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "1.Jugar", Color::Black, 40);
			jugar->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->ventana->getSize().y / 6.f + 100.f);
		}

		void loop()
		{
			while (menu_activo)
			{
				eventos();

				update();

				render();
			}

		}

		void update()
		{
			if (Keyboard::isKeyPressed(Keyboard::Enter))
				menu_activo = false;

		}

		void eventos()
		{
			while (this->ventana->pollEvent(this->evento))
			{
				if (this->evento.type == Event::Closed)
				{
					this->ventana->close();
					exit(1);
				}
			}
		}

		void render()
		{
			this->ventana->clear();

			this->ventana->draw(this->Smenu);
			this->cats_invasion->render();
			this->jugar->render();

			this->ventana->display();
			
		}

};

class Juego
{
	private:

	int altura, anchura;
	RenderWindow *ventana;
	Event evento;
	Nivel *n1;
	Menu *menu;

	public:

	Juego(int altura,int anchura)
	{
		
		this->ventana = new RenderWindow(VideoMode(altura, anchura), "Cat's Invasion");
		this->ventana->setFramerateLimit(60);
		
		n1 = new Nivel("Texturas/cuarto.jpg", ventana->getSize(),this->ventana,this->evento);
		menu = new Menu(this->ventana, ventana->getSize(), this->evento);

		loop();
	}

	void loop()
	{
		while (this->ventana->isOpen())
		{
			menu->loop();
			n1->loop();
		}
	}

	void eventos()
	{
		while (this->ventana->pollEvent(this->evento))
		{
			if (this->evento.type == Event::Closed)
			{
				this->ventana->close();
				exit(1);
			}
		}
	}

	void update()
	{
		this->n1->j->update();
	}
	
	void render()
	{
		this->ventana->clear();

		this->n1->render();
		this->n1->j->render();
		
		this->ventana->display();

	}
};

int main()
{
	Juego j(800, 640);
	
	system("pause");
	return 0;
}