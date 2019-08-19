#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Collision.hpp"
#include "conio.h"
#include "cstdlib"

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

		uvRect.width = int(textura->getSize().x / numImagenes.x);
		uvRect.height = int(textura->getSize().y / numImagenes.y);
	}

	void update(int fila, float deltaTime,bool faceRight)
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

		uvRect.top = imagenActual.y * uvRect.height;
		
		if (faceRight)
		{
			uvRect.left = imagenActual.x * uvRect.width;
			uvRect.width = abs(uvRect.width);
		}
		else
		{
			uvRect.left = (imagenActual.x + 1) * abs(uvRect.width);
			uvRect.width = -abs(uvRect.width);
		}

	}
};


class Texto{

private:
	Font fuente;
	Text texto;
	RenderWindow* ventana;
	int tama�o;
	string mensaje;
	string fuentes;
	float pos_x, pos_y;
	Color color;

public:
	Texto(RenderWindow* ventana,string fuentes,string mensaje,Color color,int tama�o)
	{
		this->ventana = ventana;
		this->mensaje = mensaje;
		this->tama�o = tama�o;
		this->fuente = fuente;
		this->color = color;

		fuente.loadFromFile(fuentes);
		texto.setFont(fuente);
		texto.setCharacterSize(tama�o);
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
		int opcion = 1, timer, delay;
		float distancia;

	public:
		Puntero(string textura,RenderWindow* ventana,float distancia,Vector2f escala,int delay)
		{
			this->textura = textura;
			this->ventana = ventana;
			this->delay = delay;
			this->distancia = distancia;
			Tpuntero.loadFromFile(textura);
			Spuntero.setTexture(Tpuntero);
			Spuntero.setScale(escala);
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
					Spuntero.move(0.f, distancia);
					opcion++;
				}
				timer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::Up) && timer == delay)
			{

				if (opcion == 1)
				{
					Spuntero.setPosition(x, y + distancia);
					opcion = 2;
				}
				else
				{
					Spuntero.move(0.f, -distancia);
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

class Personaje {
	
private:

	float velocidad;
	float tiempoAnim;
	float deltaTime = 0.0f;
	bool miraDerecha = true;
	Texture Tni�o;
	RectangleShape ni�o;
	Animacion* animacionNi�o;
	RenderWindow* ventana;

public:

	Personaje(Vector2f initPos, RenderWindow* ventana, string rutaTextura, Vector2f tama�o, float velocidad)
	{
		this->ventana = ventana;
		this->velocidad = 100.f;
		tiempoAnim = 100 / velocidad * 0.2f;
		Tni�o.loadFromFile(rutaTextura);
		ni�o.setSize(tama�o);
		ni�o.setTexture(&Tni�o);

		animacionNi�o = new Animacion(&Tni�o, Vector2u(8, 1), tiempoAnim);

		ni�o.setTextureRect(animacionNi�o->uvRect);
		ni�o.setPosition(Vector2f(0.0f,ventana->getSize().y - ni�o.getSize().y * 2.1f));
	}
	 
	void update(RenderWindow* ventana,float deltaTime)
	{
		Vector2f movimiento(0.0f, 0.0f);

		if (Keyboard::isKeyPressed(Keyboard::Left))
			if(ni�o.getPosition().x > -10.f)
				movimiento.x -= velocidad * deltaTime;

		if (Keyboard::isKeyPressed(Keyboard::Right))
			if(ni�o.getPosition().x < ventana->getSize().x)
				movimiento.x += velocidad * deltaTime;

		if (movimiento.x > 0)
			miraDerecha = true;
		else if (movimiento.x < 0)
			miraDerecha = false;

		if(movimiento.x != 0)
		{ 
			animacionNi�o->update(0, deltaTime, miraDerecha);
			ni�o.setTextureRect(animacionNi�o->uvRect);
			ni�o.move(movimiento);
		}
	}

	void render(RenderWindow* ventana)
	{
		ventana->draw(ni�o);
	}


	bool salioDePantalla()
	{
		if (ni�o.getPosition().x > ventana->getSize().x)
			return true;

		return false;
	}

};

class Enemigo {

private:
	Texture Tenemigo;
	RenderWindow* ventana;
public:
	Sprite Senemigo;
	Enemigo(RenderWindow* ventana, string textura, Vector2f escala)
	{
		this->ventana = ventana;
		Tenemigo.loadFromFile(textura);
		Senemigo.setTexture(Tenemigo);
		Senemigo.setScale(escala);
	}

	void render()
	{
		ventana->draw(Senemigo);
	}

	void setPosition(Vector2f pos)
	{
		Senemigo.setPosition(pos);
	}

	void move(Vector2f mov)
	{
		Senemigo.move(mov);
	}

	Vector2f getPosition()
	{
		return Senemigo.getPosition();
	}

	FloatRect getSize()
	{
		return Senemigo.getGlobalBounds();
	}

};

class Spawner {
private:
	
	Enemigo enemigo;
	RenderWindow* ventana;
	int spawnTimer,spawnDelay;
	float velocidad;
	float multiplicador = 60.f;
public:
	vector<Enemigo> enemigos;

	Spawner(RenderWindow* ventana, string textura, Vector2f escala, int spawnDelay,float velocidad) : enemigo(ventana,textura,escala)
	{
		this->velocidad = velocidad;
		this->spawnDelay = spawnDelay;
		this->ventana = ventana;
		spawnTimer = this->spawnDelay;
	}

	void update(float deltaTime)
	{
		for (size_t i = 0; i < enemigos.size(); i++)
		{
			enemigos[i].move(Vector2f(-velocidad * deltaTime * multiplicador, 0.0f));

			if (enemigos[i].getPosition().x < -enemigo.getSize().width)
			{
				enemigos.erase(enemigos.begin() + i);
			}
		}
				
		if (spawnTimer <= spawnDelay)
			spawnTimer++;

		if (spawnTimer > spawnDelay)
		{
			enemigo.setPosition(Vector2f(float(ventana->getSize().x),100.f + rand()%int(ventana->getSize().y - enemigo.getSize().height - 200.f)));
			enemigos.push_back(Enemigo(enemigo));
			spawnTimer = 0;
		}

	}

	void render()
	{
		for (size_t i = 0; i < enemigos.size(); i++)
		{
			enemigos[i].render();
		}
	}

};

class UIbar
{
private:
	RenderWindow* ventana;
	RectangleShape cuadrado;
	vector <RectangleShape> barra;
	Vector2f dimension, initPos;
	int numRectangulos;
	Texture textura;
	Texto texto;

public:

	UIbar(RenderWindow* ventana, Vector2f initPos, Vector2f dimension, int numRectangulos,Color colorRelleno,Color colorEsquinas,float tama�oEsquinas,string mensaje,string fuente)
		: texto(ventana,fuente,mensaje,Color::Black,20)
	{
		this->ventana = ventana;
		this->numRectangulos = numRectangulos;
		this->dimension = dimension;
		this->initPos = initPos;

		cuadrado.setSize(Vector2f(dimension.x, dimension.y));
		cuadrado.setOutlineThickness(tama�oEsquinas);
		cuadrado.setFillColor(colorRelleno);
		cuadrado.setOutlineColor(colorEsquinas);

		texto.setPos(cuadrado.getPosition().x, cuadrado.getPosition().y - texto.getSize_y() - 20.f);
		

		for (int i = 0; i < numRectangulos; i++)
		{
			cuadrado.setPosition(Vector2f(cuadrado.getOutlineThickness() + initPos.x + i * (cuadrado.getSize().x + cuadrado.getOutlineThickness()), cuadrado.getOutlineThickness() + initPos.y));
			barra.push_back(RectangleShape(cuadrado));
		}
	}

	UIbar(RenderWindow* ventana, Vector2f initPos, Vector2f dimension, int numRectangulos, string rutaTextura,string mensaje,string fuente)
		: texto(ventana,fuente, mensaje, Color::Black, 45)
	{
		this->ventana = ventana;
		this->numRectangulos = numRectangulos;
		this->dimension = dimension;
		this->initPos = initPos;

		textura.loadFromFile(rutaTextura);
		cuadrado.setTexture(&textura);
		cuadrado.setSize(Vector2f(dimension.x, dimension.y));

		texto.setPos(initPos.x, initPos.y - texto.getSize_y() - 20.f);

		for (int i = 0; i < numRectangulos; i++)
		{
			cuadrado.setPosition(Vector2f(cuadrado.getOutlineThickness() + initPos.x + i * (cuadrado.getSize().x + cuadrado.getOutlineThickness()), cuadrado.getOutlineThickness() + initPos.y));
			barra.push_back(RectangleShape(cuadrado));
		}
	}

	void RectanguloMenos()
	{
			barra.erase(barra.begin() + numRectangulos - 1);
			numRectangulos--;	
	}

	void render()
	{
		for (int i = 0; i < signed(barra.size()); i++)
		{
			ventana->draw(barra[i]);
		}

		texto.render();
	}

	FloatRect pos()
	{
		return cuadrado.getGlobalBounds();
	}

};

class Jugador {

private:

	float velocidad = 10;
	float deltaTime = 0.0f;
	bool miraDerecha = true;
	Texture Tperro;
	float multiplicador = 60.f;

	Texture TanimPerro;
	RectangleShape perro;
	Animacion* animacionPerro;

public:
	Sprite Sperro;
	int vida = 5;

	Jugador(Vector2f initPos, RenderWindow* ventana)
	{

		if (!Tperro.loadFromFile("Texturas/dogeCartoon.png"))
			cout << "No se pudo cargar la textura" << endl;

		Sperro.setTexture(Tperro);
		Sperro.setPosition(initPos);
		Sperro.setScale(Vector2f(0.1f, 0.1f));

		TanimPerro.loadFromFile("Texturas/laserAnim.png");
		perro.setSize(Vector2f(80.f, 150.f));
		perro.setTexture(&TanimPerro);

		animacionPerro = new Animacion(&TanimPerro, Vector2u(8, 1), 0.15f);

		perro.setTextureRect(animacionPerro->uvRect);
		perro.setPosition(Vector2f(0.0f, ventana->getSize().y - perro.getSize().y * 2.1f));
	}

	void update(RenderWindow* ventana, Spawner& spawn, UIbar* vidas, float deltaTime)
	{
		multiplicador = 60.f;

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (Sperro.getPosition().x > 0)
			{
				Sperro.move(-velocidad * deltaTime * multiplicador, 0.0f);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (Sperro.getPosition().x < ventana->getSize().x - Sperro.getGlobalBounds().width)
			{
				Sperro.move(velocidad * deltaTime * multiplicador, 0.0f);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (Sperro.getPosition().y > 100)
			{
				Sperro.move(0.f, -velocidad * deltaTime * multiplicador);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (Sperro.getPosition().y < ventana->getSize().y - Sperro.getGlobalBounds().height - 100)
			{
				Sperro.move(0.f, velocidad * deltaTime * multiplicador);
			}
		}

		for (size_t i = 0; i < spawn.enemigos.size(); i++)
		{
			if (Collision::PixelPerfectTest(Sperro, spawn.enemigos[i].Senemigo))
			{
				spawn.enemigos.erase(spawn.enemigos.begin() + i);
				vidas->RectanguloMenos();
				vida--;
			}
		}

		animacionPerro->update(0, deltaTime,true);
		perro.setTextureRect(animacionPerro->uvRect);

	}

	void render(RenderWindow* ventana)
	{
		ventana->draw(Sperro);
		ventana->draw(perro);
	}

};

class Bala {

private:
	Texture Tbala;
	RenderWindow* ventana;
public:
	Sprite Sbala;
	Bala(RenderWindow* ventana, string textura, Vector2f escala)
	{
		this->ventana = ventana;
		Tbala.loadFromFile(textura);
		Sbala.setTexture(Tbala);
		Sbala.setScale(escala);
	}

	void render()
	{
		ventana->draw(Sbala);
	}

	void setPosition(Vector2f pos)
	{
		Sbala.setPosition(pos);
	}

	void move(Vector2f mov)
	{
		Sbala.move(mov);
	}

	Vector2f getPosition()
	{
		return Sbala.getPosition();
	}

	FloatRect getSize()
	{
		return Sbala.getGlobalBounds();
	}

};

class SpawnerBala {
private:

	Bala bala;
	RenderWindow* ventana;
	int spawnTimer, spawnDelay;
	float velocidad;
	float multiplicador = 60.f;
	Jugador* jugador;
public:
	vector<Bala> balas;

	SpawnerBala(RenderWindow* ventana,Jugador* jugador, string textura, Vector2f escala, int spawnDelay, float velocidad) : bala(ventana, textura, escala)
	{
		this->velocidad = velocidad;
		this->spawnDelay = spawnDelay;
		this->ventana = ventana;
		this->jugador = jugador;
		spawnTimer = this->spawnDelay;
	}

	void update(float deltaTime, Spawner& spawn)
	{
		for (size_t i = 0; i < balas.size(); i++)
		{
			balas[i].move(Vector2f(velocidad * deltaTime * multiplicador, 0.0f));

			if (balas[i].getPosition().x > ventana->getSize().x)
			{
				balas.erase(balas.begin() + i);
			}
		}

		if (spawnTimer <= spawnDelay)
			spawnTimer++;

		if (Keyboard::isKeyPressed(Keyboard::Space) && spawnTimer > spawnDelay)
		{
			bala.setPosition(Vector2f(jugador->Sperro.getPosition().x + jugador->Sperro.getGlobalBounds().width / 1.5f,jugador->Sperro.getPosition().y + (jugador->Sperro.getGlobalBounds().height / 3.f)));
			balas.push_back(Bala(bala));
			spawnTimer = 0;
		}

		for (int i = 0; i < spawn.enemigos.size(); i++)
		{
			for (int j = 0; j < balas.size(); j++)
			{
				if (Collision::PixelPerfectTest(balas[j].Sbala, spawn.enemigos[i].Senemigo))
				{
					spawn.enemigos.erase(spawn.enemigos.begin() + i);
					balas.erase(balas.begin() + j);
					break;

				}
			}
		}

	}

	void render()
	{
		for (size_t i = 0; i < balas.size(); i++)
		{
			balas[i].render();
		}
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
	float deltaTime = 0.0f;
	Spawner gatos;
	Jugador* perro;
	SpawnerBala* balas;
	UIbar* barraVida;
	Music musica;

	Nivel(string ruta_fondo, Vector2u ventana_escala,RenderWindow *ventana,Event evento) 
		: gatos(ventana,"Texturas/cartoon_cat.png",Vector2f(0.2f,0.2f),30,10000)		
	{
		this->ventana = ventana;
		this->evento = evento;

		perro = new Jugador(Vector2f(40.f, 80.f), ventana);
		balas = new SpawnerBala(ventana, perro, "Texturas/laser.png", Vector2f(0.35f, 0.35f), 30, 10);
		barraVida = new UIbar(ventana,Vector2f(20.0f, ventana->getSize().y - 45.0f), Vector2f(50.f, 40.f), perro->vida,"Texturas/heart2.png","Vidas:","Fuentes/fuente_cartoon.ttf");
		
		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana_escala.x/Sfondo.getGlobalBounds().width, ventana_escala.y / Sfondo.getGlobalBounds().height));

		musica.openFromFile("Musica/nivelMusica.ogg");
		musica.setLoop(true);

	}

	void loop()
	{
		
		Clock reloj;
		srand(time(NULL));
		//musica.play();

		while (nivel_activo && perro->vida > 0)
		{
			deltaTime = reloj.restart().asSeconds();

			eventos();
			
			update();

			render();
		}
	}

	void update()
	{
		perro->update(ventana,gatos,barraVida,deltaTime);
		balas->update(deltaTime,gatos);
		gatos.update(deltaTime);
	}

	void eventos()
	{
		while (ventana->pollEvent(evento))
		{
			if (evento.type == Event::Closed)
			{
				ventana->close();
				exit(1);
			}

			if (evento.key.code == Keyboard::P)
				musica.play();

			if (evento.key.code == Keyboard::O)
				musica.pause();
		}
	}

	void render()
	{
		ventana->clear();
		
		ventana->draw(Sfondo);
		perro->render(ventana);
		gatos.render();
		balas->render();
		barraVida->render();
		ventana->display();
	}
};

class Escena
{
public:
	Texture Tfondo;
	Sprite Sfondo;

	RenderWindow* ventana;

	Event evento;

	Texto historia;

	bool intro_activo = true;
	float deltaTime = 0.0f;

	Personaje ni�o;

	Escena(string ruta_fondo, Vector2u ventana_escala, RenderWindow* ventana, Event evento) :
		historia(ventana, "Fuentes/fuente_elegante.ttf", "Esta es la historia de billy, \nun ni�o que solo quer�a ser feliz.",Color::Black, 80) 
		, ni�o(Vector2f(40.f, 80.f), ventana, "Texturas/ni�oAnim.png",Vector2f(100.f, 150.f),100.f)
	{
		this->ventana = ventana;
		this->evento = evento;

		historia.setPos(50.f, 50.f);

		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana_escala.x / Sfondo.getGlobalBounds().width, ventana_escala.y / Sfondo.getGlobalBounds().height));

	}

	void loop()
	{
		Clock reloj;

		while (intro_activo)
		{
			deltaTime = reloj.restart().asSeconds();

			eventos();

			update();

			render();
		}
	}

	void update()
	{
		ni�o.update(ventana,deltaTime);

		if (ni�o.salioDePantalla())
			intro_activo = false;
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
		ventana->clear();

		ventana->draw(this->Sfondo);
		ni�o.render(ventana);
		historia.render();

		ventana->display();
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
		Texture TcatMenu;
		Sprite ScatMenu;

	public:
		Puntero* p1;

		Menu(RenderWindow *ventana, Vector2u ventana_escala, Event evento)
		{
			this->ventana = ventana;
			this->evento = evento;
			Tmenu.loadFromFile("Texturas/menu.jpg");
			Smenu.setTexture(Tmenu);
			Smenu.setScale(Vector2f(ventana_escala.x / Smenu.getGlobalBounds().width, ventana_escala.y / Smenu.getGlobalBounds().height));


			cats_invasion = new Texto(this->ventana, "Fuentes/fuente_titulo.ttf", "Cat's Invasion", Color::Blue,100);
			cats_invasion->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f, this->ventana->getSize().y / 6.f);

			TdogeMenu.loadFromFile("Texturas/dogeMenu.png");
			SdogeMenu.setTexture(TdogeMenu);
			SdogeMenu.setScale(0.2f, 0.2f);			
			SdogeMenu.setPosition(cats_invasion->getPos_x() + cats_invasion->getSize_x() * 1.01f, cats_invasion->getPos_y() + this->cats_invasion->getSize_y() - this->SdogeMenu.getGlobalBounds().height / 1.5f);

			TcatMenu.loadFromFile("Texturas/catMenu.png");
			ScatMenu.setTexture(TcatMenu);
			ScatMenu.setScale(SdogeMenu.getGlobalBounds().width / ScatMenu.getGlobalBounds().width, SdogeMenu.getGlobalBounds().height / ScatMenu.getGlobalBounds().height);
			ScatMenu.setPosition(cats_invasion->getPos_x() - ScatMenu.getGlobalBounds().width * 1.1f, cats_invasion->getPos_y() + this->cats_invasion->getSize_y() - this->ScatMenu.getGlobalBounds().height / 1.5f);

			jugar = new Texto(this->ventana, "Fuentes/fuente_elegante.ttf", "Jugar", Color::Black, 80);
			jugar->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->cats_invasion->getPos_y() + this->cats_invasion->getSize_y() + 50);

			salir = new Texto(this->ventana, "Fuentes/fuente_elegante.ttf", "Salir", Color::Black, 80);
			salir->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->ventana->getSize().y / 6.f + 200.f);

			p1 = new Puntero("Texturas/apuntador.png",ventana, (salir->getPos_y() + (salir->getSize_y() / 2.0f) - (jugar->getPos_y() + jugar->getSize_y() / 2.0f)), Vector2f(0.3f, 0.3f), 20);
			p1->setPos(jugar->getPos_x() - p1->getSize_x(), jugar->getPos_y() + jugar->getSize_y() - p1->getSize_y() / 2);
			p1->setInitPos(jugar->getPos_x() - p1->getSize_x(), jugar->getPos_y() + jugar->getSize_y() - p1->getSize_y() / 2);
		}

		void loop()
		{
			menu_activo = true;

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
			ventana->clear();

			ventana->draw(Smenu);
			cats_invasion->render();
			ventana->draw(SdogeMenu);
			ventana->draw(ScatMenu);
			p1->render();
			jugar->render();
			salir->render();

			ventana->display();
			
		}
};

class Juego
{
	private:

	int altura, anchura;
	RenderWindow *ventana;
	Event evento;
	Escena *introduccion;
	Nivel* n1;
	Menu *menu;
	bool jugando = true;

	public:

	Juego(int altura,int anchura)
	{
		
		ventana = new RenderWindow(VideoMode(altura, anchura), "Cat's Invasion");
		ventana->setFramerateLimit(60);
		ventana->setPosition(Vector2i(250, 50));
		ventana->setVerticalSyncEnabled(true);
		
		introduccion = new Escena("Texturas/cartoon_city.jpg", ventana->getSize(),this->ventana,this->evento);
		n1 = new Nivel("Texturas/cartoon_room.jpg", ventana->getSize(), this->ventana, this->evento);
		menu = new Menu(this->ventana, ventana->getSize(), this->evento);

		loop();
	}

	void loop()
	{
		
		while (ventana->isOpen() && jugando)
		{
			menu->loop();

			switch (menu->p1->getOpc())
			{
				case 1:
					//introduccion->loop();
					n1->loop();
					break;
				case 2:
					ventana->close();
					jugando = false;
					break;
				default:
					break;
			}
		}
	}
};

int main()
{
	Juego j(800, 640);

	return 0;
}