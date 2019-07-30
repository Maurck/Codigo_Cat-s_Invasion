#include <iostream>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

class Animacion {
private:
	Vector2u numImagenes;
	Vector2u imagenActual;

	float tiempoTotal;
	float tiempoCambio;

public:
	IntRect uvRect;

	Animacion(Texture* textura, Vector2u numImagenes, float tiempoCambio)
	{
		this->numImagenes = numImagenes;
		this->tiempoCambio = tiempoCambio;
		tiempoTotal = 0.0f;

		imagenActual.x = 0;

		uvRect.width = textura->getSize().x / float(numImagenes.x);
		uvRect.height = textura->getSize().y / float(numImagenes.y);
	}

	void update(int fila, float deltaTime)
	{
		imagenActual.y = fila;
		tiempoTotal += deltaTime;

		if (tiempoTotal >= tiempoCambio)
		{
			tiempoTotal -= tiempoCambio;
			imagenActual.x++;
		}

		if (imagenActual.x >= numImagenes.x)
			imagenActual.x = 0;

		uvRect.left = imagenActual.x * uvRect.width;
		uvRect.top = imagenActual.y * uvRect.height;

		cout << deltaTime << endl;
	}
};


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

	float getPos_x()
	{
		return this->texto.getPosition().x;
	}

	float getPos_y()
	{
		return this->texto.getPosition().y;
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
		int opcion = 1, timer, delay = 10;

	public:
		Puntero(string textura,RenderWindow* ventana)
		{
			this->textura = textura;
			this->ventana = ventana;
			Tpuntero.loadFromFile(textura);
			Spuntero.setTexture(Tpuntero);
			Spuntero.setScale(Vector2f(0.2, 0.2));
			timer = delay;
		}

		void update()
		{
			if (timer < delay)
				timer++;
				
			if (Keyboard::isKeyPressed(Keyboard::Down) && timer == delay)
			{
				
				if (opcion == 2)
				{
					Spuntero.setPosition(x, y);
					opcion = 1;
				}
				else
				{
					Spuntero.move(0.f, 50.f);
					opcion++;
				}
				timer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::Up) && timer == delay)
			{

				if (opcion == 1)
				{
					Spuntero.setPosition(x, y + 50);
					opcion = 2;
				}
				else
				{
					Spuntero.move(0.f, -50.f);
					opcion--;
				}
				timer = 0;
			}

		}

		void render()
		{
			this->ventana->draw(this->Spuntero);
		}

		float getSize_x()
		{
			return this->Spuntero.getGlobalBounds().width;
		}

		float getSize_y()
		{
			return this->Spuntero.getGlobalBounds().height;

		}
		
		void setPos(float x, float y)
		{
			Spuntero.setPosition(x, y);
		}

		void setInitPos(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		int getOpc()
		{
			return this->opcion;
		}
};

class Jugador {
	
private:
	
	float x, y;
	float velocidad=7;
	float deltaTime = 0.0f;

public:
	
	Texture Tperro;
	Sprite Sperro;
	RenderWindow* ventana;
	

	Jugador(float x, float y, RenderWindow* ventana)
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
			if(Sperro.getPosition().x > 0)
			{
				Sperro.move(-velocidad, 0.f);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (Sperro.getPosition().x < ventana->getSize().x - Sperro.getGlobalBounds().width)
			{
				Sperro.move(velocidad, 0.f);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (Sperro.getPosition().y > 0)
			{
				Sperro.move(0.f, -velocidad);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (Sperro.getPosition().y < ventana->getSize().y - Sperro.getGlobalBounds().height)
			{
				Sperro.move(0.f, velocidad);
			}
		}
	}

	void render()
	{
		this->ventana->draw(Sperro);
	}

};

class Nivel
{
public:
	Texture Tfondo;
	Sprite Sfondo;
	Texture Tniño;
	RectangleShape Sniño;
	Animacion* niño;
	RenderWindow* ventana;
	Event evento;
	bool nivel_activo = true;
	float deltaTime = 0.0f;
	Clock reloj;

	Jugador *j;

	Nivel(string ruta_fondo, Vector2u ventana_escala,RenderWindow *ventana,Event evento)
	{
		this->ventana = ventana;
		this->evento = evento;

		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana_escala.x/Sfondo.getGlobalBounds().width, ventana_escala.y / Sfondo.getGlobalBounds().height));
		
		j = new Jugador(40, 80, ventana);

		Tniño.loadFromFile("Texturas/niñoAnim.jpg");
		Sniño.setSize(Vector2f(300.f, 400.f));
		Sniño.setTexture(&Tniño);

		niño = new Animacion(&Tniño, Vector2u(9,1), 0.1f);
	}

	void loop()
	{
		while (nivel_activo)
		{
			deltaTime = reloj.restart().asSeconds();

			eventos();
			
			update();

			render();
		}
	}

	void update()
	{
		this->j->update();
		niño->update(0, deltaTime);
		Sniño.setTextureRect(niño->uvRect);
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
		this->ventana->draw(Sniño);

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
		Texto *cats_invasion,*jugar,*salir;

		Texture TdogeMenu;
		Sprite SdogeMenu;

	public:
		Puntero* p1;

		Menu(RenderWindow *ventana, Vector2u ventana_escala, Event evento)
		{
			this->ventana = ventana;
			Tmenu.loadFromFile("Texturas/menu.jpg");
			Smenu.setTexture(Tmenu);
			Smenu.setScale(Vector2f(ventana_escala.x / Smenu.getGlobalBounds().width, ventana_escala.y / Smenu.getGlobalBounds().height));

			cats_invasion = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "Cat's Invasion", Color::Blue, 50);
			cats_invasion->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f, this->ventana->getSize().y / 6.f);

			TdogeMenu.loadFromFile("Texturas/dogeMenu.png");
			SdogeMenu.setTexture(TdogeMenu);
			SdogeMenu.setScale(0.2f, 0.2f);			
			SdogeMenu.setPosition(cats_invasion->getPos_x() + cats_invasion->getSize_x() + 10, cats_invasion->getPos_y() -  SdogeMenu.getGlobalBounds().height / 5);

			jugar = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "1.Jugar", Color::Black, 40);
			jugar->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->ventana->getSize().y / 6.f + 100.f);

			p1 = new Puntero("Texturas/apuntador.png", this->ventana);
			p1->setPos(this->jugar->getPos_x() - p1->getSize_x(), this->jugar->getPos_y() + this->jugar->getSize_y() / 4);
			p1->setInitPos(this->jugar->getPos_x() - p1->getSize_x(), this->jugar->getPos_y() + this->jugar->getSize_y() / 4);

			salir = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "2.Salir", Color::Black, 40);
			salir->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->ventana->getSize().y / 6.f + 150.f);
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

			this->p1->update();
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
			this->ventana->draw(this->SdogeMenu);
			this->p1->render();
			this->jugar->render();
			this->salir->render();

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
	bool jugando = true;

	public:

	Juego(int altura,int anchura)
	{
		
		this->ventana = new RenderWindow(VideoMode(altura, anchura), "Cat's Invasion");
		this->ventana->setFramerateLimit(60);
		this->ventana->setPosition(Vector2i(250, 50));
		
		n1 = new Nivel("Texturas/cuarto.jpg", ventana->getSize(),this->ventana,this->evento);
		menu = new Menu(this->ventana, ventana->getSize(), this->evento);

		loop();
	}

	void loop()
	{
		
		while (this->ventana->isOpen() && jugando)
		{
			menu->loop();

			switch (this->menu->p1->getOpc())
			{
				case 1:
					n1->loop();
					break;
				case 2:
					this->ventana->close();
					jugando = false;
					break;
				default:
					break;
			}
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
		
		this->ventana->display();

	}
};

int main()
{
	Juego j(800, 640);

	return 0;
}