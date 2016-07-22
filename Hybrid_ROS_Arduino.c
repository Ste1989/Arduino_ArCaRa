/*
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

 /*We need to include the ros.h as with any other ROS Arduino program.
 We also include the header files for messages, in this case, the Empty message.
 */
#include <ros.h>
#include <std_msgs.h>


#define PI 3.14159
#define CPR 16384 		//# tacche
#define mod_lettura 1 	//lettura 1x

#define num_buff 100	//buffer per stima velocità

#define T_c 5000 		//tempo di campionamento 0.5 millisecondi
#define resol_stat 100	//risoluzione per numeri float

#define A 2				//canale A encoder
#define B 7				//canale B encoder
#define INDEX 13		//canale Index encoder

#define PWM_ENABLE 8	
#define PWM_INPUT1 3
#define PWM_INPUT2 11

//////////////////////////////////// VAR
/* volatile long int counter, counter_old;
volatile char B_new;
float theta,theta_old,delta_theta, omega, somma, buff[num_buff],delta_t,passo;
long int W_omega;
long int Theta;
int j;
//VAR TIME
unsigned long start_time, new_time, start_time_old;
//VAR ATTUAZIONE e INIZIALIZZAZIONE
int cont_battuta;
char flag_start;
int duty;
//VAR VARIE
int i;
char app; */
int flag;


/*We need to instantiate the node handle, which allows our program
to create publishers and subscribers.
The node handle also takes care of serial port communications.*/
ros::NodeHandle nh;

/*We then create the callback function for our subscriber.
The call back function must take a constant reference of a message as
its argument. In our callback messageCb, the type of message is
std_msgs::Empty and the message name will be toggle_msg.*/
void messageCb( const std_msgs::String& toggle_msg){
  //digitalWrite(13, HIGH-digitalRead(13));   // blink the led
  
  
  //qui leggo il messaggio che sarà del tipo APRI/CHIUDI
  //magari imposto una variabile
  if (toggle_msg.data == "APRI")
	  flag = 1;
  else
	  flag = 0;
  
}

/*We need to instantiate the publishers and subscribers that we will
be using. Here we instantiate a Subscriber with a topic name of
"toggle_led" and type std_msgs::Empty. With Subscribers,
you must remember to template the subscriber upon the message.
Its two arguments are the topic it will be subscribing to and the callback
function it will be using.*/
ros::Subscriber<std_msgs::String> sub("toggle_led", &messageCb );


/*In the Arduino setup function you then need to initialize your ROS
node handle, advertise any topics being published,
and subscribe to any topics you wish to listen to.*/
void setup()
{
	
	init_PWM();
	//init_TIME();
	//ENCODER
	/*attacco all'interrupt 0 il segnale A e all'interrupt 1 il segnale B*/
	//attachInterrupt(0,A,RISING); //INT0 --> PIN 2
	
  //pinMode(13, OUTPUT);
  nh.initNode(); //pensa anche alla seriale
  nh.subscribe(sub);
}

/*Finally, in the loop function we call ros::spinOnce()
where all of the ROS communication callbacks are handled.
We don't need to do any additional processing in the loop(),
since ros::spinOnce() will handle passing messages to the subscriber
callback.*/
void loop()
{
	
	init();
	
  nh.spinOnce();
  //delay(1);
  //qui devo fare il controllo su quella variabile flag impostata in  
  //messageCb
  //A questo punto:
  //1.battuta meccanica
  //2.pwm con controllo su angolo
  //2.1.ci vuole gran parte del vecchio codice
	if flag == 1
		set_OCRx(duty);
	else
		set_OCRx(-duty);
}

/////////////////////////////////////////////////////////////////////////////////

void init()
{
	
	flag = 0;
	
  /* if (flag_start == 0)
  {
    init_battutameccanica();
    //se è la prima volta che esegue
    start_time = micros();
    flag_start = 1;
  } */
 
}


void init_PWM(){
  //configurazione timer2 per generazione di due canali PWM. i due canali si trovano sui PIN 3 e 11.
  //variando il valore di OCR2x è possibile cambiare il valore del duty. OCR2A è relativo al PIN 11, 
  //l'altro al PIN 3
  //duty cicle: 0 : 0% = 255 : 100%
  duty = 50;
  pinMode(PWM_INPUT1,OUTPUT);
  pinMode(PWM_INPUT2,OUTPUT);
  pinMode(PWM_ENABLE,OUTPUT);
  TCCR2A= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20) ;
  //TCCR2B= _BV(CS20); //no prescaler  62 Khz
  TCCR2B= _BV(CS21); // prescaler  8  7 Khz
  //TCCR2B= _BV(CS21) | _BV(CS20); // prescaler  32
  //TCCR2B= _BV(CS20); // prescaler  64
  //..datasheet
  OCR2A= 127; //50%
  OCR2B=  127; //50%
  digitalWrite(PWM_ENABLE,HIGH); 
}

void set_OCRx(int d)
{
	//dovrebbe essere quella che effettivamente fa partire il segnale PWM
  int ocra = (255 * d )/100;  
  int ocrb= 255-ocra;
  
  OCR2A =ocra;
  OCR2B =ocrb;
 }
 
/*  void init_ENCODER()
{
	pinMode(A, INPUT);
	pinMode(B, INPUT);  
	pinMode(INDEX,INPUT);
	counter = 0;
	counter_old=0;
	B_new=0;
	delta_theta=0;
	theta=0;
	theta_old=0;
}

void init_TIME()
{
  start_time=0;
  new_time=0;
  delta_t=0;
  start_time_old=0;
} */

/* void init_battutameccanica()
{
  set_OCRx(25);
  delay(500);
  while(1){
    //Serial.println(cont_battuta);
    theta_old = theta;
    theta=counter * passo;
    if(theta == theta_old)
      cont_battuta++;
    else
      cont_battuta = 0;
    if(cont_battuta == 50)//fine_corsa
      break;
   } 
   //INIZIALIZZO
   set_OCRx(50);
   theta = theta_old = 0;
   counter = counter_old = 0;
   //Serial.println("OK");
} */