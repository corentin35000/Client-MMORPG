#include "engine.h"


Engine::Engine(int screenWidth, int screenHeight) {
	screen_width = screenWidth; // Screen window width
	screen_height = screenHeight; // Screen window height

	//icon = al_load_bitmap("assets/misc/minimap/minimap.png");

	// Données qui sera pour créer le Timer avec Allegro
	time_t rawtime;
	char timestr[256];
	time(&rawtime);
	tm time = {};
	localtime_s(&time, &rawtime);
	strftime(timestr, 256, "%c", &time);

	// Initialisation de la lib Allegro
	if (! al_init())
	{
		printf("Failed to init Allegro. \n");
	}

	// Initialise le module d'image d'Allegro.
	if (! al_init_image_addon())
	{
		printf("Failed to init IIO addon. \n");
	}

	// Initialise le module de primitives d'Allegro
	al_init_primitives_addon();

	// Initialise le module de Font d'Allegro
	al_init_font_addon();

	// Initialisation le module de police TTF d'Allegro
	al_init_ttf_addon();

	// Obtenez le nombre d'adaptateurs vidéo connectés à l'ordinateur.
	al_get_num_video_adapters();

	// Display / Monitor -> Récupère un objet ALLEGRO_MONITOR_INFO puis -> Récupère le moniteur 0 (principale du pc)
	ALLEGRO_MONITOR_INFO info;
	al_get_monitor_info(0, &info);
	al_set_new_display_option(ALLEGRO_SUPPORTED_ORIENTATIONS, ALLEGRO_DISPLAY_ORIENTATION_ALL, ALLEGRO_SUGGEST);
	int flags = ALLEGRO_FULLSCREEN; // ALLEGRO_RESIZABLE | ALLEGRO_WINDOWED | ALLEGRO_FULLSCREEN | ALLEGRO_FULLSCREEN_WINDOW;
	al_set_new_display_flags(flags);

	// Crée la fenetre d'écran window
	display = al_create_display(screen_width, screen_height);

	// Title window game
	al_set_window_title(display, "Sea Tyrants");
	
	// Icon window game
	//al_set_display_icon(display, icon);
	
	// Test si la creation de la fenetre window a etait creer.
	if (! display)
	{
		printf("Error creating display.\n");
	}
	
	// Taux de HZ du moniteur.
	//fprintf(stderr, "fresh rate : %d", al_get_display_refresh_rate(display));

	// Installez un pilote de clavier.Renvoie vrai en cas de succès
	if (! al_install_keyboard())
	{
		printf("Error installing keyboard.\n");
	}

	// Installez un pilote de souris. Renvoie vrai en cas de succès
	if (! al_install_mouse())
	{
		printf("Error installing mouse.\n");
	}

	// Load 2 fonts et de type TTF.
	font.push_back(al_load_font("assets/fonts/segoeui.ttf", -14, ALLEGRO_TTF_MONOCHROME));
	font.push_back(al_load_font("assets/fonts/arial.ttf", -16, ALLEGRO_TTF_MONOCHROME));


	// Creation du Timer de la boucle de jeu et pour la gestion des evenements Allegro (pour 60 fps : 1/60 = 0.016ms)
	timer = al_create_timer(1.0 / FPS);

	// Crée une file d'attente d'événements avec al_create_event_queue()
	// Puis indiquez aux sources d'entrée de placer de nouveaux événements dans cette file d'attente à l'aide de al_register_event_source()
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());

	// Initialisation de la librairie : ENET (sockets UDP)
	if (enet_initialize() != 0)
	{
		printf("ENET non initialiser !");
	}

	// Create client (ENET / UDP)
	/*client = enet_host_create(NULL, 1, 2, 0, 0);
	if (client == NULL)
	{
		printf("Erreur create Client-ENET \n");
	}

	// Set Host et établie la connection au server.
	enet_address_set_host(&address, "51.91.214.24");
	address.port = 56289;
	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL)
	{
		fprintf(stderr, "Aucun homologue disponible pour l'établissement d'une connexion ENet \n");
	}

	// TENTATIVE DE CONNECTION AU SERVER (Entre deux sockets UDP) : Attendez jusqu'à 5 secondes pour que la tentative de connexion réussisse
	if (enet_host_service(client, &eventENET, 5000) > 0 && eventENET.type == ENET_EVENT_TYPE_CONNECT)
	{
		printf("Connection to server. \n");
		enet_peer_ping_interval(peer, 0);
		enet_peer_ping(peer);
	}
	else
	{
		enet_peer_reset(peer); 
		printf("Connection server failed. \n");
	}*/

	// Démarrez la minuterie spécifiée.
	// À partir de là, le compteur du minuteur s'incrémentera à un rythme constant et commencera à générer des événements (event ci-dessus)
	// C'est le nombre de fois par seconde que le programme vas s'executer a partir du moment ou on démare le timer 
	al_start_timer(timer);
}

Engine::~Engine() {

}