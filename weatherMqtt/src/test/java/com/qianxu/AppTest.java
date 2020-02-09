package com.qianxu;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import com.qianxu.weather.Weather;
import com.qianxu.weather.WeatherImpl;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit test for simple App.
 */
public class AppTest {
    @Before
    public void setUpStreams() {
        System.setOut(new PrintStream(new ByteArrayOutputStream()));
        System.setErr(new PrintStream(new ByteArrayOutputStream()));
    }

    /**
     * Rigorous Test :-)
     */
    @Test
    public void shouldAnswerWithTrue() {
        Weather weather = new WeatherImpl();
        weather.init();
        System.out.println(weather.getWeather());
    }
}
