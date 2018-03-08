#define UDRIEMASK   0b00100000
#define F_CPU 16000000UL

// TODO: Check if we can use serial interrupt to interrupt for the comms b/w xbees or computer to basestation arduino
// also check to see if we can use the other serial port for a faster comms 

// TODO: Try a method of sending stop bytes and start bytes so to start a sentence it will start with # and end with /n - for continuous streams

char dataRecv, dataSend;

// Completes receiving
ISR( USART0_RX_vect )
{
    // Write received data to dataRecv
    dataRecv = UDR0;
    UCSR0B |= 0b00100000;
    sendData(dataRecv); // Sending the character back
}

// Complete sending
ISR( USART0_UDRE_vect )
{
    // Write dataSend to UDR0
    // Disable UDRE interrupt
    UCSR0B &= 0b11011111; // Reset UDRIE to 0
    
}


void sendData(const char data)
{
    // Copy data to be sent to dataSend
    //dataSend = buttonVal+'0';
    
    
    // Enable UDRE interrupt below
    
    while ((UCSR0A & 0b00100000) == 0);
    UDR0 = data;
}

char recvData()
{
    unsigned char data;
    while ((UCSR0A & 0b10000000) == 0);
    data = UDR0;
    
    
    return data;
}

void setupSerial()
{
    // Serial Monitor to test
    UCSR0A = 0;
    UCSR0C = 0b00000110;
    // Aync mode 
    // disabled parity
    // 0 stop bit
    // 8 bit char size
    // Non polling mode     
}

void startSerial()
{
    
    UCSR0B = 0b11011000;
    // Start the serial port.
    // Enable RXC interrupt and TXC, but NOT UDRIE
    // Remember to enable the receiver
    // and transmitter
}

void setBaud(unsigned long baudRate)
{
    unsigned int b;
    b = (unsigned int) round(F_CPU / (16.0 * baudRate))- 1;
    UBRR0H = (unsigned char) (b >> 8);
    UBRR0L = (unsigned char) b;
}


void setup() {
    // put your setup code here, to run once:
    cli();
    setupSerial();
    setBaud(9600);
    startSerial();
    sei();
}



void loop() {
    // put your main code here, to run repeatedly:
    
}

