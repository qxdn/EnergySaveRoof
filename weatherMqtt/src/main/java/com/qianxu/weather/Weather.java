package com.qianxu.weather;


import com.qianxu.dto.WeatherMsg;

public interface Weather{
    public void init();
    public void init(String location);
    public WeatherMsg getWeather(String location);
    public WeatherMsg getWeather();
}