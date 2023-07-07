#pragma once

namespace  CppCLRWinFormsProject {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            InitializeComponent();
        }

    protected:
        ~Form1()
        {
            if (components)
            {
                delete components;
            }
        }

    private: System::ComponentModel::Container^ components;
    private: array<PictureBox^>^ cards;
    private: Random^ random;
    private: PictureBox^ selectedCard;
    private: Timer^ timer;
    private: int moves;
    private: bool matchFound;
    private: Button^ restartButton; // Botón para reiniciar la actividad

#pragma region Windows Form Designer generated code
           void InitializeComponent(void)
           {
               this->restartButton = (gcnew System::Windows::Forms::Button());
               this->SuspendLayout();
               // 
               // restartButton
               // 
               this->restartButton->Location = System::Drawing::Point(525, 371);
               this->restartButton->Name = L"restartButton";
               this->restartButton->Size = System::Drawing::Size(75, 23);
               this->restartButton->TabIndex = 0;
               this->restartButton->Text = L"Restart";
               this->restartButton->UseVisualStyleBackColor = true;
               this->restartButton->Click += gcnew System::EventHandler(this, &Form1::restartButton_Click);
               // 
               // Form1
               // 
               this->ClientSize = System::Drawing::Size(612, 415);
               this->Controls->Add(this->restartButton);
               this->Name = L"Form1";
               this->Text = L"Memory Game";
               this->BackgroundImage= Image::FromFile("C:/Users/wilte/Downloads/Imagenes/Fondo.png");
               this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
               this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
               this->ResumeLayout(false);
           }
#pragma endregion

           // Perform any additional initialization
           void Form1_Load(System::Object^ sender, System::EventArgs^ e)
           {
               cards = gcnew array<PictureBox^>(12); // Update the array size to accommodate all the cards
               random = gcnew Random();
               selectedCard = nullptr;
               timer = gcnew Timer();
               moves = 0;
               matchFound = false;

               // Create PictureBoxes for the cards
               for (int i = 0; i < cards->Length; i++)
               {
                   cards[i] = gcnew PictureBox();
                   cards[i]->Size = System::Drawing::Size(100, 100);
                   cards[i]->Location = System::Drawing::Point(50 + (i % 4) * 120, 50 + (i / 4) * 120);
                   cards[i]->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                   cards[i]->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
                   cards[i]->Click += gcnew System::EventHandler(this, &Form1::Card_Click);
                   cards[i]->Tag = i / 2; // Assign a unique tag to each card
                   cards[i]->Image = Image::FromFile("C:/Users/wilte/Downloads/Imagenes/Reverso.png"); // Set the back of the card as the default image
                   this->Controls->Add(cards[i]);

               }

               ShuffleCards();
           }

           // Shuffle the cards
           void ShuffleCards() // se encarga de barajar las cartas en el juego de memoria. 
           {
               array<int>^ duplicatedIndices = gcnew array<int>(cards->Length);

               // Generate duplicated indices
               for (int i = 0; i < duplicatedIndices->Length; i++)
               {
                   duplicatedIndices[i] = i / 2;
               //Se genera una serie de índices duplicados en el arreglo duplicatedIndices.|
               //Cada valor en el arreglo se calcula dividiendo el índice entre 2, lo que significa que cada par de cartas tendrá el mismo valor de índice.
               }

               // Shuffle the duplicated indices (Barajar cartas)
               for (int i = 0; i < duplicatedIndices->Length; i++)
               {
                   int temp = duplicatedIndices[i];
                   int randomIndex = random->Next(duplicatedIndices->Length);
                   duplicatedIndices[i] = duplicatedIndices[randomIndex];
                   duplicatedIndices[randomIndex] = temp;
               }

               // Assign the images to the cards
               for (int i = 0; i < cards->Length; i++)
               {
                   cards[i]->Tag = duplicatedIndices[i];
               }
           }

           // Handle card click event and check if we have all cards with his pair.
           void Card_Click(System::Object^ sender, System::EventArgs^ e)
           {
               PictureBox^ clickedCard = dynamic_cast<PictureBox^>(sender);
               //Obtener una referencia al objeto PictureBox real que generó el evento,
               //lo que te permite interactuar con ese objeto de manera específica dentro del controlador de eventos.
               
               // Ignore if the clicked card is already matched or currently selected
               if (!clickedCard->Visible || clickedCard == selectedCard)
                   return;

               // Reveal the image on the clicked card
               clickedCard->Image = GetCardImage(clickedCard->Tag->ToString()); //Habiamos definido un tag para cada imagen, 
               //este lo convertimos a string y entra como referencia al id de imagen que queremos mostrar.
               clickedCard->Refresh();

               if (selectedCard == nullptr) //Usamos carta seleccionada y carta clickeada, dando referencia a dos imagenes.
                //El puntero vacio indica que no se ha tomado la primera imagen,  por la cual esta pasa a ser la primera imagen "SELECTEDCARD" y la segunda
               // viene a ser "clickedcard" esto es importante para cuando hagamos el match de cartas.
               {
                   selectedCard = clickedCard;

               }
               else
               {
                   // Check if the selected card matches the clicked card
                   if (selectedCard->Tag->ToString() == clickedCard->Tag->ToString())
                   {
                       // Correct match
                       selectedCard->Visible = false; 
                       clickedCard->Visible = false;
                       //Si las dos son iguales, entonces ambas se vuelven invisibles, desaparecen del tablero.
                   }
                   else
                   {
                       // Incorrect match, hide the cards again
                       System::Threading::Thread::Sleep(1000); // Delay for 1 second
                       //In this part, we put again the Reverso because the selected and clicked card aren't similar.
                       selectedCard->Image = Image::FromFile("C:/Users/wilte/Downloads/Imagenes/Reverso.png");// Set the back of the card as the default image
                       clickedCard->Image = Image::FromFile("C:/Users/wilte/Downloads/Imagenes/Reverso.png");// Set the back of the card as the default image
                   }
                   selectedCard = nullptr; // Establece el puntero selectedCard como nulo para indicar que no hay ninguna carta seleccionada actualmente
                   moves++;

                   // Check if all cards are matched
                   bool allMatched = true;
                   for (int i = 0; i < cards->Length; i++)
                   {
                       if (cards[i]->Visible)
                       {
                           allMatched = false;
                           break;
                       }
                   }

                   if (allMatched)
                   {
                       timer->Stop();
                       if (MessageBox::Show("Congratulations! You won in " + moves + " moves. Do you want to restart?", "Game Over", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::Yes)
                       {
                           // Reset the game
                           for (int i = 0; i < cards->Length; i++)
                           {
                               cards[i]->Visible = true;
                               cards[i]->Image = Image::FromFile("C:/Users/wilte/Downloads/Imagenes/Reverso.png");
                           }
                           ShuffleCards();
                           moves = 0;
                           selectedCard = nullptr;
                           timer->Start();
                       }
                       else
                       {
                           this->Close();
                       }
                   }
               }
           }

           // Get the corresponding image for the card based on the tag value
           Image^ GetCardImage(String^ tag)
           {
               if (tag == "0")
                   return Image::FromFile("C:/Users/wilte/Downloads/Imagenes/1.png");// Set the back of the card as the default image
               else if (tag == "1")
                   return Image::FromFile("C:/Users/wilte/Downloads/Imagenes/2.png");// Set the back of the card as the default image
               else if (tag == "2")
                   return Image::FromFile("C:/Users/wilte/Downloads/Imagenes/3.png");// Set the back of the card as the default image
               else if (tag == "3")
                   return Image::FromFile("C:/Users/wilte/Downloads/Imagenes/4.png");// Set the back of the card as the default image
               else if (tag == "4")
                   return Image::FromFile("C:/Users/wilte/Downloads/Imagenes/5.png");// Set the back of the card as the default image
               else if (tag == "5")
                   return Image::FromFile("C:/Users/wilte/Downloads/Imagenes/6.png");// Set the back of the card as the default image
               else
                   return nullptr;
           }
           // Handle restart button click event
           void restartButton_Click(System::Object^ sender, System::EventArgs^ e)
           {
               // Reset the game
               for (int i = 0; i < cards->Length; i++)
               {
                   cards[i]->Visible = true;
                   cards[i]->Image = Image::FromFile("C:/Users/wilte/Downloads/Imagenes/Reverso.png");
               }
               ShuffleCards();
               moves = 0;
               selectedCard = nullptr;
               timer->Start();
           }
           // Handle form closing event
           void Form1_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
           {
               // Clean up resources
               delete random;
               delete timer;
           }
       };
 }





