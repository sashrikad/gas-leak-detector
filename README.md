# Gas Leak Detector


<p align="center">
  <kbd>
  
  [![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/ITzezCxTu54/0.jpg)](https://www.youtube.com/watch?v=ITzezCxTu54)

</kbd>
</p>


## Description
I have designed "Gas Leak Detector" which is built on Seeed's Wio Terminal. It 's connected with Multichannel gas sensor and on Fan. I am running a tinyML model on the microcontroller to detect if there is any smell of diesel in the air. When diesel is detected, I am sending a push notification through Blynk app.



## Why did you choose to make this project?

Oil tank leakage is serious problem. People living in cold weather like east coast of the USA, use diesel to heat up the house during winter. Typically oil tank has a life span around 15 to 20 years. If regular maintainance if not done, cracks start to a appear. Most of the old houses have oil tank in their basements. Basement is a place where people usually don't visit everyday. So if there is any leakage in the oil tank, changes are that, they will be unnoticed. This can lead to serious damage including catching fire. Early detection of the leakage can prevent high repair cost as well as prevent any chances of fire.

Using technology we can sense the leakage and provide realtime notification to the owner. Using machine learning and multi channel gas sensor, a device can identify if there is any diesel or petrol in the air and alert.

## What did you find difficult while making your project and how did you work it out?

I have faced some issues and resolved them to get to the final prototype. The most interesting one was adding the fan. Before I added fan, my ML model was not able to predict timely. When I was shaking the diesel container, it was able to pick the smell. So I realized a fan can circulate the air and improve my device. 
Another issue I faced was imporoper classification. As I initially captured data from a different place than basement, due to other smell like from cooking, coffee etc, my ML model was classifying them as leakage. So I had to capture varity of data for my ML model to make better prediction. 

## Is there anything you would do differently or you would add if you had more time?

If I had more time, then I would probably add a solar pannel because there are lots of houses that have outdoor oil tanks. If we add solar pannels to it, then it can last long outside. Another thing I would add if I had time would be to collect data for different gas since not all oil tanks have the same gas inside. Like some can have deisel, petrol, and more.

### Project on Hackster 
https://www.hackster.io/mithun-das/oil-tank-leak-detection-using-tinyml-by-sashrika-29222f

