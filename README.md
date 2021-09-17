***Technology Stack:***

1. IBM Watson Studio
2. IBM Watson Machine Learning
3. IBM Watson IOT Platform
4. NodeRED Application
5. Arduino IDE

This project has been divided into 2 phases:
1. Pre-plantation 
2. Post-plantation

***Pre-plantation Process: ML Model*** 

Data for different crops regarding their optimum requirements of N, P, K (Nitrogen, Phosphorus, Potassium) and pH levels in the soil along with average temperature and rainfall needed is curated and mould into a proper dataset which is then used to train our Machine Learning model to predict the most appropriate crop to be grown. 

The algorithm used is Random Forest Mutli-classification Algorithm for better accuracy than Decision Tree.

Accuracy: 0.9933

The ML model has been connected to a Web Application developed using NodeRED Platform with the help of an API.

***Post-plantation Process: IoT Model***

Once the farmer decides which crop he wants to grow, a Continuous Monitoring and Smart Irrigation System along with UI has been developed using IoT to keep the crops health in check.

The three components used in this process are:
1. NodeMCU Microcontroller for data transfer
2. Soil Moisture sensor to calculate water level in soil
3. DHT11 sensor to calculate Temperature and Humidity around the crop.

This system has been connected to a IBM Watson IoT Platform, which helps in updating the UI with most recent values.

