[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Apache-2.0 License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
[![Kickstarter][kickstarter-shield]][kickstarter-url]

[kickstarter-shield]: https://img.shields.io/badge/kickstarter-00C300?style=for-the-badge&logo=kickstarter&logoColor=white
[kickstarter-url]: https://www.kickstarter.com/projects/731479134/pedro-0


[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=blue
[linkedin-url]: https://linkedin.com/in/almoutazar-saandi

[contributors-shield]: https://img.shields.io/github/contributors/almtzr/Pedro.svg?style=for-the-badge&colorB=red
[contributors-url]: https://github.com/almtzr/Pedro/graphs/contributors

[forks-shield]: https://img.shields.io/github/forks/almtzr/Pedro.svg?style=for-the-badge&colorB=yellow
[forks-url]: https://github.com/almtzr/Pedro/network/members

[stars-shield]: https://img.shields.io/github/stars/almtzr/Pedro.svg?style=for-the-badge&colorB=orange
[stars-url]: https://github.com/almtzr/Pedro/stargazers

[license-shield]: https://img.shields.io/github/license/almtzr/Pedro.svg?style=for-the-badge&colorB=286
[license-url]: https://github.com/almtzr/Pedro/blob/main/LICENSE

---
# Last Kickstarter Actu :

## 📅 19 March 2025
### 🚀 Three Ways to Get Your Own Pedro Robot!✨

Greetings to all Pedro supporters! 🤖 <br>
Thanks for being part of the Pedro adventure! 🚀

As you know, you can get Pedro in different [Kickstarter!](https://www.kickstarter.com/projects/731479134/pedro-0) rewards. Choose the one that suits you best!
Whether you backed a Pre-Assembled Pedro, a Full DIY Kit, or a Pedro Board, it comes preloaded with the [pedroDirectMode.ino](https://github.com/almtzr/Pedro/tree/main/code/directMode) firmware, allowing you to control Pedro in Direct Mode like this:

<div align="left">
    <img src="img/pedro_direct_mode.gif" width="80%">
</div>

⚡ 1. Pre-Assembled Pedro Reward - Plug & Play
Want a fully built and tested Pedro? The Pre-Assembled Pedro is ready to use just turn it on and explore your Pedro in action.

🔧 2. Full DIY Kit Reward - Everything Included
With the Pedro Full DIY Kit, you get all the necessary components to build the robot, including electronics and 3D-printed parts. Just follow the Get Started guide on GitHub and start assembling!

🛠️ 3. Pedro Board Only Reward - Print & Build
Want to build Pedro from scratch? The Pedro Board Only reward gives you the main PCB, and you can 3D print the rest! To complete your robot, you’ll need:

- 2 ball bearings 8x22x7mm
- 7.4V DC battery with JST XH2.54 connector 2 pins
- 4x MS90 360° servo motors
- Micro USB cable
- (Options) Modules: NRF24L01, HC-05, ESP8266-01, OLED 128x64 0.96"

All assembly instructions are available in the [Get Started guide](https://github.com/almtzr/Pedro-Get-Started).

💬 Join the Pedro Community!
Need help? Have questions? Want to share your build? Join our Discord server to connect with other makers, get support, and follow Pedro’s journey in real time!

---

# Pedro Robot

Pedro is a sophisticated mini robotic arm designed to be entirely 3D printed and assembled without any tools. Equipped with four mini servo motors and powered by a custom-made Arduino-compatible board, Pedro offers versatile wireless communication options with its integrated **nRF24L01 module for remote control**, **HC-05 Bluetooth module for smartphone connectivity**, and **ESP8266 WiFi module for IoT integration**. It also features an **OLED 128x64 display**, allowing real-time feedback and data visualization.

<div align="left">
    <img src="img/Pedro_Cover.png" width="100%">
</div>

<br>

Since 2025, Pedro Robot is certified as open-source hardware by the Open Source Hardware Association under UID: [FR000025](https://certification.oshwa.org/fr000025.html).

To ensure smooth and accurate operations, Pedro is equipped with two ball bearings, strategically placed for enhanced precision in its movements. This versatile and educational robot is perfect for hobbyists, students, and makers looking to explore robotics, electronics, and programming, all while enjoying a hands-on and engaging experience.

<div align="center">
    <img src="img/pedro_overview.jpeg" width="100%">
</div>

## 1. Features

Pedro is a compact, DIY robot that you can build and program yourself. All parts of Pedro are designed to be 3D printed, and the robot can be assembled without any additional tools.

</br>

- **4 Mini Servo Motors**: For precise movements.
- **Arduino-Compatible Board**: Designed specifically for this project, offering easy integration with the robot's components.
- **Two Rechargeable 18650 Batteries**: Powering the robot, rechargeable via USB.
- **Fully 3D Printable**: All structural components can be printed on a standard 3D printer.
- **Tool-Free Assembly**: Designed to snap together easily without the need for tools.
- **Micro USB Cable**: For Charging and Programming.

## 2. Boards

The Pedro Board is here to revolutionize how you learn and create in electronics, programming, and robotics. Designed with simplicity and versatility in mind, this open-source board comes in two versions: the **Pedro Rev 3** with 4 powerful integrated modules (**NRF24L01**, **ESP8266-01**, **HC-05**, and **OLED 128x64**) and the **Pedro Rev 2** with 2 integrated modules (**NRF24L01** and **OLED 128x64**). Both versions offer a compact and efficient design to help you bring your robotic projects to life!

Additionally, the **Gerber files for the Pedro Rev 2** are available for those who want to create their own custom Pedro robot board.

<div align="left">
    <img src="img/pedro_board_rev3.png" width="80%">
</div>

<br>

- **OLED Screen (128x64)**: Visualize data, debug in real-time, or create interactive menus.
- **NRF24L01**: Enable long-range wireless communication between Pedro or devices.
- **ESP8266-01 WiFi Module**: Bring your Pedro online with ease. (**Rev 3 only**)
- **HC-05 Bluetooth Module**: Connect wirelessly to smartphones or other devices. (**Rev 3 only**)

## 3. How to Build Your Pedro

Checkout the [Pedro Get Started](https://github.com/almtzr/Pedro-Get-Started/tree/main) repo to bring your Pedro to life. You'll find detailed assembly instructions, Programming guides and much more.

## 4. Applications

### Radio communication by NRF24L01 module

<div align="left">
    <img align="center" src="img/dual-mode.gif" width="80%">
</div>

### Bluetooth communication by HC-O5 module

 <div align="left">
    <img align="center" src="img/bluetooth_mode.gif" width="80%">
</div>
 
### WiFi communication by ESP8266-01 module
 ... work in progress ...

## 5. Contributing
We welcome contributions from the community! Here's how you can help:

1. **Fork the Repository**: Click the "Fork" button at the top right of this page.
2. **Clone Your Fork**: 
   ```
   git clone https://github.com/almtzr/Pedro.git
   ```
3. **Create a Branch**: 
   ```
   git checkout -b feature/your-feature-name
   ```
4. **Make Your Changes**: Add new features, fix bugs, or improve documentation.
5. **Commit and Push**: 
   ```
   git commit -m "Add your message here"
   git push origin feature/your-feature-name
   ```
6. **Submit a Pull Request**: Navigate to the original repository and submit a pull request.

### Explore the Incredible Possibilities

The programming possibilities are endless, providing an opportunity for both beginners and advanced users to push the boundaries of what Robot Pedro can achieve. Enjoy the process of learning, experimenting, and refining your programming skills with this remarkable STEAM education tool.

<div align="center">
    <img src="img/workshop.jpg" width="100%">
</div>

## 6. License
This project is licensed under the Apache-2.0 License. See the `LICENSE` file for more details.

---

> Hi, I'm Almoutazar SAANDI, computer engineer based in France. In my free time, I enjoy creating open-source projects, and Pedro is one of them. 
If you need more details or have any questions about Pedro, feel free to reach out to me.

---
