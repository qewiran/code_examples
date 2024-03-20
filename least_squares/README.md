## МНК системой ортогональных многочленов

Задана табличная функция $f(x)$ на отрезке [-1 1]. С помощью *ортогонализации Грама-Шмидта* получаем ортогональный полиномиальный базис $(q_0,...,q_{order})$ до заданного порядка (в задании пятый). Ортогонализацию определим рекуррентным соотношением
$$q_{k+1} = xq_k + \alpha_{k+1} q_k + \beta_k q_{k-1}$$

где (сумма не рендерится нормально на гитхабе)

$$\begin{align}
&\alpha_{k+1} =\frac{(xq_k,q_k)} {(q_k,q_k)} = \frac{ \sum_{i=1}^{\mathrm{nodeCount}} x_i q_k^2 (x_i)}{\sum_{i=1}^{\mathrm{nodeCount}} q_k^2(x_i)}\\
&\beta_{k} = \frac{(xq_k,q_{k-1})}{(q_{k-1},q_{k-1})} = \frac{ \sum_{i=1}^{\mathrm{nodeCount}} x_iq_k(x_i)q_{k-1}(x_i)}{\sum_{i=1}^{\mathrm{nodeCount}} q_{k-1}^2(x_i)} 
\end{align} $$

Далее получаем *коэффициенты Фурье*:

$$c_j = \frac{(q_j(x),f(x))}{(q_j(x),q_j(x))} = \frac{\sum_{i=1}^{\mathrm{nodeCount}} q_j(x_i)f(x_i)}{\sum_{i=1}^{\mathrm{nodeCount}} q_j^2(x_i)}$$ 

и составляем *полином наилучшего среднеквадратичного приближения*:

$$P(x) = \sum_{j=1}^{\mathrm{order}} c_jq_j(x)$$
