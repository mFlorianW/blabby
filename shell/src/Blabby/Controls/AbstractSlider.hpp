// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "AbstractInteractiveControl.hpp"

namespace Shell
{

/**
 * Implementation logic of a Slider Control.
 */
class AbstractSlider : public AbstractInteractiveControl
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * The minimum of the slider range. Default 0.0
     */
    Q_PROPERTY(qreal from READ from WRITE setFrom NOTIFY fromChanged)

    /**
     * The upper range of the slider range. Default 100.0
     */
    Q_PROPERTY(qreal to READ to WRITE setTo NOTIFY toChanged)

    /**
     * The value of the slider depending of the current position
     */
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)

    /**
     * The readonly property the visual position of slider.
     * It's used for interal purpose only
     */
    Q_PROPERTY(qreal visualPosition READ visualPosition NOTIFY visualPositionChanged)

public:
    /**
     * Creates an instance of the @ref Shell::AbstractSlider.
     */
    AbstractSlider();
    /**
     * Default destructor
     */
    ~AbstractSlider() override;
    Q_DISABLE_COPY_MOVE(AbstractSlider)

    /**
     * @return The lower range of the control.
     */
    qreal from() noexcept;

    /**
     * Sets the lower range of the slider.
     * @param The new lower range of slider.
     */
    void setFrom(qreal from) noexcept;

    /**
     * @return The upper range of the control.
     */
    qreal to() const noexcept;

    /**
     * Sets the upper range of the slider.
     * @param to The new upper range of the slider.
     */
    void setTo(qreal to) noexcept;

    /**
     * @return Gives the current value of the slider.
     */
    qreal value() const noexcept;

    /**
     * Sets the new value of the slider
     * Is the new value bigger then the @ref Shell::AbstractSlider::to value the value is set @ref
     * Shell::AbstractSlider::to.
     * @param value The new value of the slider.
     */
    void setValue(qreal value) noexcept;

    /**
     * @return Gives the visual position of the slider.
     * Only used for internal purpose.
     */
    qreal visualPosition() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the from value is changed.
     */
    void fromChanged();

    /**
     * This signal is emitted when the to value is changed.
     */
    void toChanged();

    /**
     * This signal is emitted when the value is changed.
     */
    void valueChanged();

    /**
     * This signal is emitted when the step size is changed.
     */
    void stepSizeChanged();

    /**
     * This signal is emitted when the visual position is changed.
     */
    void visualPositionChanged();

protected:
    void handleRelease(QPointF point) noexcept override;
    void handleMove(QPointF point) noexcept override;

private Q_SLOTS:
    void onWidthChanged() noexcept;

private:
    qreal getVisualPosition(QPointF point) noexcept;

    qreal mFrom = 0.0;
    qreal mTo = 100.0;
    qreal mValue = 0.0;
    qreal mStepSize = 1.0;
    qreal mVisualPositon = 0.0;
};

} // namespace Shell
