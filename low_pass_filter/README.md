# Реализация ФНЧ Баттерворта

## Вывод передаточной функции

Даны величины: граничные частоты полосы пропускания в герцах: верхняя — `freqStop`, нижняя — `freqPass`, коэффициенты передачи полосы пропускания `gainPass` в децибелах, коэффициенты передачи полосы подавления `gainStop` в децибелах.

Требуется по этим величинам реализовать частотный фильтр, чтобы подавлялись частоты вне полосы пропускания.

Реализуем Фильтр Баттерворта, отличительной особенностью которого является гладкая частотная характеристика.

По определению у этого фильтра АЧХ имеет вид: 

$$
|H(i\omega )| = \frac{1}{\sqrt{ 1+ \left( \frac{\omega}{\omega_{c}} \right)^{2n} }}
$$

где $\omega_c$ — частота среза, $n$ — порядок фильтра.

Причем нормализованную АЧХ (когда $\omega_c=1$) обозначим так:

$$
|H_{n}(i\omega )| = \frac{1}{\sqrt{ 1+ \omega ^{2n}  }}
$$

Примем для вывода передаточной функции параметр $s \in \mathbb{C}$, такой что $\mathrm{Re}\,s=0$, то есть $s=i\omega$. Возведем в квадрат нормализованную АЧХ: 

$$
|H_{n}(i\omega)|^2 = H_{n}(i\omega)H_{n}(-i\omega) = \frac{1}{1+\omega^{2n}}
$$

Так как $\omega = \frac{s}{i}$, получаем: 

$$
|H_{n}(i\omega)|^2 = \frac{1}{1+\left( \frac{s}{i} \right) ^{2n}}
$$

Отсюда получаем полюса, тогда когда $$\left( \frac{s}{i} \right)^{2n}=-1 \implies s^{2n} = -1 \cdot i^{2n}=e^{i\pi(2k-1)} \cdot e^{2n \cdot i\pi/2 } = e^{i\pi(2k-1+n)} $$ Возьмем корень $2n$ из обеих частей тождества, получим корни:

$$s_{k} =  e\huge^{\frac{i\pi}{2n}(2k+n-1)} \normalsize = \cos \left( \frac{2k+n-1}{2n}\pi \right) + i \sin \left( \frac{2k+n-1}{2n}\pi \right)  \tag{1}$$

Итого у нас вышло $2n$ корней на единичной окружности, берем только те, что в левой полуплоскости для устойчивости фильтра. 

Итого передаточная функция имеет вид 

$$
H_{n}(s) = \frac{1}{(s-s_{1})\ldots(s-s_{n})} = \frac{1}{s^n+a_{n-1}s_{n-1}+\ldots+a_{1}s+1} = \frac{1}{B_{n}(s)}
$$

где $B_{n}(s)$ — полином Баттерворта.

Чтобы получить передаточную функцию для частоты среза не равной 1 Гц, просто делим переменную на $\omega_c$:

$$
H(s) = H_{n}\left( \frac{s}{\omega _{c}} \right) = \frac{1}{B_{n}\left( \frac{s}{\omega_{c}} \right)}
$$

## Определение нужного порядка фильтра и частоты среза (если не задана)

Коэффициент передачи выражен в децибелах:

$$
G = 20 \log_{10}|H(i\omega)| = 20 \log_{10}\left( \frac{1}{\sqrt{ 1+ \left( \frac{\omega}{\omega_{c}} \right)^{2n}  }} \right) = -10\log_{10}\left({ 1+ \left( \frac{\omega}{\omega_{c}} \right)^{2n} } \right)  
$$

Для заданных значений `freqStop`, `freqPass`, `gainStop`, `gainPass`:

$$
\begin{align}
&G_{\mathrm{pass}} = -10\log_{10}\left( 1+\left( \frac{\omega_{\mathrm{pass}}}{\omega_{c}} \right)^{2n}  \right) \implies \left( \frac{\omega_{\mathrm{pass}}}{\omega_{c}} \right)^{2n} = 10^{-G_{\mathrm{pass}}/10} -1 \\

&G_{\mathrm{stop}} = -10\log_{10}\left( 1+\left( \frac{\omega_{\mathrm{stop}}}{\omega_{c}} \right)^{2n}  \right) \implies \left( \frac{\omega_{\mathrm{stop}}}{\omega_{c}} \right)^{2n} = 10^{-G_{\mathrm{stop}}/10} -1 \\

& \left( \frac{\omega_{\mathrm{stop}}}{\omega_{\mathrm{pass}}} \right)^{2n} = \frac{10^{-G_{\mathrm{stop}}/10} -1}{ 10^{-G_{\mathrm{pass}}/10} -1} \implies n = \frac{\log_{10}\left(\frac{10^{-G_{\mathrm{stop}}/10} -1}{ 10^{-G_{\mathrm{pass}}/10} -1} \right)}{2 \log_{10}\left( \frac{\omega_{\mathrm{stop}}}{\omega_{\mathrm{pass}}} \right) } 
\end{align} 
$$



