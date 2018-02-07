# Ninja LS Tool v0.3.3

This is a Spyware - NinjaLS - I developed for Ethical Hacking and Penetration Testing purposes. This tool was built with information-privacy responsibility in mind and its meant to be used on machines that the attacker has permission to conduct penetration testing or any other forms of information security testing. I will not be held liable for any deviations and usage of this tool for criminal purposes. 

This tool creates data (Keystrokes, Mouse Clicks, User Activity) from the NinjaLS Spyware tool and curates them into a dashboard where the attacker/penetration tester can sieve through using a Windows-based Analytics tool I created in another project - [Decrypt NinjaLS](https://github.com/jinwei908/NinjaLSDecrypt-public).

This has been deployed on the Amazon Cloud so it can be accessed from anywhere, anytime.

*This project is done while learning about C++ and is the first time I am using this language.*

Please visit my [Blog](https://www.reversethatshell.com) for more information and screenshots of this NinjaLS Tool.
- [Blog - NinjaLS Web Framwork](https://www.reversethatshell.com/2018/02/05/ninja-web-framework-keylogger-data-crunching-dashboard/)
- [Blog - NinjaLS Spyware Tool](https://www.reversethatshell.com/2017/07/23/keylogger-and-analysis-console-penetration-testing-tool/)

## Getting Started

These set of instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

*To be implemented once the build is stable*

### Prerequisites

**For Local Development**

This framework needs a C++ Compiler (preferably CodeBlocks)
- MingW Threading

You need to include a file: config.h into the root directory.
This config.h should contain the below information:

```
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <map>
#include <string>
#include <vector>

std::map<std::string, std::string> CONFIG_DICT{
    {"SERVER_HOST", "xxx.xxx.com"},
    {"LOCAL_HOST", "127.0.0.1:8000/"},
    {"EMAIL_ENABLED", "TRUE"},
    {"SERVER_ENABLED", "FALSE"},
    {"LOG_TIMER_API", "FALSE"},
    {"API_DATA", "/api-auth/post/data/"},
    {"API_HOST_LOG", "/api-auth/post/hostlog/"},
    {"API_HOST_IMAGE", "/api-auth/post/imageupload/"},
    {"USER_NAME", "xxx"},
    {"PASSWORD", "xxx"},
    {"SCREENSHOT_ENABLED", "FALSE"},
    {"GMAIL_LOGIN", "xxx@gmail.com"},
    {"GMAIL_PASSWORD", "xxx"}
};


#endif // CONFIG_H_INCLUDED

```

* Replace XXX where applicable, after which, compile and build with Code Blocks.
* If this tool is not used in conjunction with Ninja Web Framework, only *EMAIL_ENABLED*, *GMAIL_LOGIN*, *GMAIL_PASSWORD* should be edited
* If you are looking to use this tool instead of changing the code, a Kali Linux payload generator is coming real soon, stay tuned!

### Installing

*To be implemented once the build is stable*

## Deployment

*To be implemented once the build is stable*

## Built With

* C++ - The Language Used

## Contributing

*To be implemented once the build is stable*

## Versioning

This built is current on v0.3.x as of 12th August 2017

## Future Improvements

This sections shows the future improvements and enhancements that are planned for this tool.
* Kali Linux Integration with Payload Generation
* More Filters
* Graphical Logs
* User Friendly Keystrokes

## Authors

* **Chong Jin Wei** - [ReverseThatShell.com](https://www.reversethatshell.com)

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/jinwei908/NinjaLS-public/blob/master/LICENSE) file for details